package main

import (
	"flag"
	"fmt"
	"io/ioutil"
	"net/http"
	"net/url"
	"time"
)

var num int
var URL string
var proxy string

func init() {
	flag.IntVar(&num, "num", 10, "number of client")
	flag.StringVar(&proxy, "proxy", "127.0.0.1:8000", "proxy address")
	flag.StringVar(&URL, "url", "www.baidu.com", "url of the website you want to access")
	flag.Parse()
}

func main() {
	errChan := make(chan error, 1)
	sucChan := make(chan bool, num)
	for i := 0; i < num; i++ {
		go func(i int) {
			get(i, errChan, sucChan)
		}(i)
	}

	sucNum := 0
	tick := time.After(time.Minute)
	for {
		select {
		case err := <-errChan:
			fmt.Println("error: ", err)
			return
		case <-sucChan:
			sucNum++
			if sucNum == num {
				fmt.Println("success!")
				return
			}
		case <-tick:
			fmt.Println("gorutine timeout!")
			return
		}
	}

}

func get(idx int, errChan chan error, sucChan chan bool) {
	urli := url.URL{}
	urlproxy, err := urli.Parse("//" + proxy)
	if err != nil {
		errChan <- err
		return
	}

	c := http.Client{
		Transport: &http.Transport{
			Proxy: http.ProxyURL(urlproxy),
		},
		Timeout: 5 * time.Second,
	}
	resp, err := c.Get("http://" + URL)
	if err != nil {
		errChan <- err
		return
	}

	if err := writeToFile(idx, resp); err != nil {
		errChan <- err
		return
	}

	sucChan <- true
}

func writeToFile(idx int, resp *http.Response) error {
	defer resp.Body.Close()

	data, err := ioutil.ReadAll(resp.Body)
	if err != nil {
		return err
	}

	filename := fmt.Sprintf("%d.out", idx)
	if err := ioutil.WriteFile(filename, data, 0644); err != nil {
		return err
	}

	return nil
}
