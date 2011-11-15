# Nginx variable request_speed

This module create variable $request_speed contain time of request. Good chooice to track backend (Apache, php-fpm) performance.
Enable by var_request_speed_filter on;

Example log format:

log_format combined2 '$remote_addr - $remote_user [$time_local] "$request" $status $body_bytes_sent "$http_referer" "$http_user_agent" $gzip_ratio $request_speed';

Example log:
18x.16x.8x.25x - - [15/Nov/2011:06:36:02 -0500] "GET /db/login.php HTTP/1.1" 200 0 "http://domain.com/db/" "Mozilla/5.0 (Linux; U; Android 2.3.3; en-us; Sprint APA9292KT Build/GRI40) AppleWebKit/533.1 (KHTML, like Gecko) Version/4.0 Mobile Safari/533.1" - 353 0.200

email: alexstup@gmail.com
