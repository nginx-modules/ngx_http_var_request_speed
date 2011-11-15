# Nginx variable request_speed

This module create variable $request_speed contain time of request. Good chooice to track backend (Apache, php-fpm) performance.
Enable by var_request_speed_filter on;

Example log format:

log_format combined2 '$remote_addr - $remote_user [$time_local] "$request" $status $body_bytes_sent "$http_referer" "$http_user_agent" $gzip_ratio $request_speed';

email: alexstup@gmail.com
