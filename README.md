log_format combined2 '$remote_addr - $remote_user [$time_local] "$request" $status $body_bytes_sent "$http_referer" "$http_user_agent" $gzip_ratio $slowfs_cache_status $request_speed';
var_request_speed_filter on;
