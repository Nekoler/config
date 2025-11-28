socat TCP-LISTEN:8080,fork,reuseaddr TCP-CONNECT:127.0.0.1:8000 &
socat TCP-LISTEN:8081,fork,reuseaddr TCP-CONNECT:127.0.0.1:8001 &
