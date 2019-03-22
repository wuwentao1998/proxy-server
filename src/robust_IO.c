#include "robust_IO.h"

/*
 * EFFECTS: internal helper function
 *          wrapper for the Unix read()
 *          transfer min(n, rio_cnt) bytes from an internal buffer to a user
 *          buffer, where n is the number of bytes requested by the user and
 *          rio_cnt is the number of unread bytes in the internal buffer.
 */
static ssize_t rio_read(rio_t* rp, char* usrbuf, size_t n)
{
    while (rp->rio_cnt <= 0)
    {
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));

        if (rp->rio_cnt < 0)
        {
            if (errno != EINTR)
                return -1;
        }
        else if (rp->rio_cnt == 0)
            return 0;
        else
            rp->rio_bufptr = rp->rio_buf;
    }

    int cnt = n;
    if (rp->rio_cnt < n)
        cnt = rp->rio_cnt;

    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

/*
 * EFFECTS: wrapper funtion for rio_readn
 *          add error handling
*/
ssize_t Rio_readn(int fd, void *usrbuf, size_t n)
{
    ssize_t bytes = rio_readn(fd, usrbuf, n);

    if (bytes < 0)
        unix_error("Rio_readn error");

    return bytes;
}

/*
 * EFFECTS: read n bytes data from file with descriptor fd to usrbuf
 *          return the number of data actually read or 0 if encounters EOF
 *          this function is unbuffered
 * ERRORS: return -1 if encounters error
*/
ssize_t rio_readn(int fd, void *usrbuf, size_t n)
{
    size_t num_left = n;
    ssize_t nread;
    char* ptr = usrbuf;

    while (num_left > 0)
    {
        if ((nread = read(fd, ptr, num_left)) < 0)
        {
            if (errno == EINTR) // interrupted by signal
                nread = 0; // call read again
            else
                return -1;
        }
        else if (nread == 0) // EOF
            break;

        num_left -= nread;
        ptr += nread;
    }

    return n - num_left;
}

/*
 * EFFECTS: wrapper funtion for rio_writen
 *          add error handling
*/
void Rio_writen(int fd, void *usrbuf, size_t n)
{
    if (rio_writen(fd, usrbuf, n) != n)
        unix_error("Rio_writen error");
}

/*
 * EFFECTS: write n bytes data from usrbuf to file with descriptor fd
 *          return the number of data actually write
 *          this function is unbuffered
 * ERRORS: return -1 if encounters error
*/
ssize_t rio_writen(int fd, void *usrbuf, size_t n)
{
    size_t num_left = n;
    ssize_t nwrite;
    char* ptr = usrbuf;

    // 由于接收方的限制（可能滑动窗口满了，write可能暂时输出不完
    // 此时应该重启write
    while (num_left > 0)
    {
        if ((nwrite = write(fd, ptr, num_left)) <= 0)
        {
            if (errno == EINTR) // interrupted by signal
                nwrite = 0; // call write again
            else
                return -1;
        }

        num_left -= nwrite;
        ptr += nwrite;
    }

    return n;
}

/*
 * EFFECTS: wrapper funtion for rio_readinitb
 *          no effect, just to unify the call interface
*/
void Rio_readinitb(rio_t *rp, int fd)
{
    rio_readinitb(rp, fd);
}

/*
 * EFFECTS: Associate a file descriptor with a read buffer and reset buffer
*/
void rio_readinitb(rio_t *rp, int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

/*
 * EFFECTS: wrapper funtion for rio_readnb
 *          add error handling
*/
ssize_t Rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
    ssize_t bytes = rio_readnb(rp, usrbuf, n);

    if (bytes < 0)
        unix_error("Rio_readnb error");

    return bytes;
}

/*
 * EFFECTS: read n bytes data to usrbuf
 *          return the number of data actually read or 0 if encounters EOF
 *          this function is buffered
 * ERRORS: return -1 if encounters error
*/
ssize_t	rio_readnb(rio_t *rp, void *usrbuf, size_t n)
{
    size_t num_left = n;
    ssize_t nread;
    char* ptr = usrbuf;

    while (num_left >0)
    {
        if ((nread = rio_read(rp, ptr, num_left)) < 0)
            return -1; // errno set by read
        else if (nread == 0)
            break; // EOF

        num_left -= nread;
        ptr += nread;
    }

    return n - num_left;
}

/*
 * EFFECTS: wrapper funtion for rio_readlineb
 *          add error handling
*/
ssize_t Rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    ssize_t bytes = rio_readlineb(rp, usrbuf, maxlen);

    if (bytes < 0)
        unix_error("Rio_readlineb error");

    return bytes;
}

/*
 * EFFECTS: read n bytes data to usrbuf
 *          return the number of data actually read or 0 if encounters EOF
 *          this function is buffered
 * ERRORS: return -1 if encounters error
*/
ssize_t	rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
{
    int bytes;
    char c;
    char* ptr = usrbuf;

    int n = 1;
    for (; n < maxlen; n++)
    {
        if ((bytes = rio_read(rp, &c, 1)) == 1)
        {
            *ptr++ = c;
            if (c == '\n')
            {
                n++;
                break;
            }
        }
        else if (bytes == 0)
        {
            if (n == 1)
                return 0; //EOF, no data read
            else
                break; //EOF, data readed
        }
        else
            return -1; // error
    }

    *ptr = '\0';

    return n - 1;
}




