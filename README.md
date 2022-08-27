# Basic-Http-Server-Cpp
HTTP server implemented in C++ without any third party library.

* Only used standard libraries

## Features

* Implemented only using GNU C/C++ Library
* Handles GET/POST requests
* Asynchronous request handling
* Multithreaded/Thread Pooling
* Handles over 20,000 concurrent connections.
* Serves over 100,000 requests per second on a modern personal computer
## Compile and Run

* Compile with a C++11 compliant compiler:
```sh
make
./server
```
### Run
It is basic dictionary app. With 2 functions add word to dictionary and check if word contains in the dictionary

* Check word if it is in the dictionary  with Get Request 
```sh
http://127.0.0.1/check?name=EXAMPLE_WORD
```
* Add word to the dictionary with Post Request 
```sh
http://127.0.0.1/add
Body should contain raw text => "name=test_word"
```

![](https://i.imgur.com/9H5LdpH.png)







## Perfonmance Results

Tested in Ubuntu 18.04 , Intel Core m-5y70 2.6GHZ-8GB-256SSD 
* Benchmarking http://127.0.0.1/
```
kayacan@therion:/home/kayacan#  ab -c 10000 -n 100000 http://127.0.0.1/
This is ApacheBench, Version 2.3 <$Revision: 1807734 $>

Benchmarking 127.0.0.1 (be patient)

Server Hostname:        127.0.0.1
Server Port:            80

Document Path:          /
Document Length:        468 bytes

Concurrency Level:      10000
Time taken for tests:   3.385 seconds
Complete requests:      100000
Failed requests:        0
Total transferred:      53500000 bytes
HTML transferred:       46800000 bytes
Requests per second:    29545.55 [#/sec] (mean)
Time per request:       338.461 [ms] (mean)
Time per request:       0.034 [ms] (mean, across all concurrent requests)
Transfer rate:          15436.39 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  115 385.7     39    3121
Processing:     1   39  46.1     42     327
Waiting:        0   30  40.9     31     309
Total:          3  154 400.0     83    3248

```


* Benchmarking Get Request http://127.0.0.1/check?name=EXAMPLE
```
kayacan@therion:/home/kayacan#  ab -c 10000 -n 100000 localhost/check?name=EXAMPLE
This is ApacheBench, Version 2.3 <$Revision: 1807734 $>


Server Software:        
Server Hostname:        localhost
Server Port:            80

Document Path:          /check?name=EXAMPLE
Document Length:        498 bytes

Concurrency Level:      10000
Time taken for tests:   3.059 seconds
Complete requests:      100000
Failed requests:        0
Total transferred:      56500000 bytes
HTML transferred:       49800000 bytes
Requests per second:    32691.55 [#/sec] (mean)
Time per request:       305.889 [ms] (mean)
Time per request:       0.031 [ms] (mean, across all concurrent requests)
Transfer rate:          18037.82 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:       35  122 183.8     84    1148
Processing:    37  100  37.0     93     429
Waiting:       10   52  41.3     41     395
Total:        125  222 198.9    216    1480
```



* Benchmarking Post Request http://127.0.0.1/add (body="Add_Keyword") #I did not try to add different words yet
```
kayacan@therion:/home/kayacan#  ab -c 10000 -p t -n 100000 http://127.0.0.1/add
This is ApacheBench, Version 2.3 <$Revision: 1807734 $>

Server Software:        
Server Hostname:        127.0.0.1
Server Port:            80

Document Path:          /add
Document Length:        468 bytes

Concurrency Level:      10000
Time taken for tests:   3.093 seconds
Complete requests:      100000
Failed requests:        0
Total transferred:      53500000 bytes
Total body sent:        13100000
HTML transferred:       46800000 bytes
Requests per second:    32332.34 [#/sec] (mean)
Time per request:       309.288 [ms] (mean)
Time per request:       0.031 [ms] (mean, across all concurrent requests)
Transfer rate:          16892.38 [Kbytes/sec] received
                        4136.27 kb/s sent
                        21028.65 kb/s total

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        6   87 216.7     36    1100
Processing:     7   43  24.0     40     265
Waiting:        4   26  19.4     24     253
Total:         23  130 220.5     75    1172

```
