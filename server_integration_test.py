import subprocess
import os
import signal
import socket
import time

# Spawn a shell process to run the server
run_server_command = ["./server", "path_config"]
server_proc = subprocess.Popen(run_server_command, stdout=subprocess.PIPE)
test_failed = False

# Echo handler test
# Spawn a shell process to curl to echo handler endpoint
echo_request_command = "curl -i -s localhost:8080/foo"
echo_request_proc = subprocess.Popen(echo_request_command, stdout=subprocess.PIPE, shell=True)
echo_response = echo_request_proc.stdout.read().decode('utf-8')

expected_echo_response = """HTTP/1.1 200 OK\r
Content-Type: text/plain\r
\r
GET /foo HTTP/1.1\r
User-Agent: curl/7.35.0\r
Host: localhost:8080\r
Accept: */*\r\n\r\n"""

if echo_response != expected_echo_response:
    print("ERROR: Echo handler replied with an incorrect response.")
    test_failed = True;
else:
    print("SUCCESS: Echo handler replied with the correct response.")

# Static File handler test: mapping static2
# Spawn a shell process to curl to static file handler endpoint with path under static2/
static2_file_request_command = "curl -i -s localhost:8080/static2/test.txt"
static2_file_request_proc = subprocess.Popen(static2_file_request_command, stdout=subprocess.PIPE, shell=True)
static2_response = static2_file_request_proc.stdout.read().decode('utf-8')

expected_static2_response = """HTTP/1.1 200 OK\r
Content-Type: text/plain\r
\r
Test file this is\n"""

if static2_response != expected_static2_response:
    print("ERROR: Static handler replied with an incorrect response for static2/.")
    test_failed = True;
else:
    print("SUCCESS: Static handler replied with the correct response for static2/.")

# Static File handler test: mapping static1
# Spawn a shell process to curl to static file handler endpoint with path under static1/
static1_file_request_command = "curl -i -s localhost:8080/static1/test.txt"
static1_file_request_proc = subprocess.Popen(static1_file_request_command, stdout=subprocess.PIPE, shell=True)
static1_response = static1_file_request_proc.stdout.read().decode('utf-8')

expected_static1_response = """HTTP/1.1 200 OK\r
Content-Type: text/plain\r
\r
HEY!\n\n"""

if static1_response != expected_static1_response:
    print("ERROR: Static handler replied with an incorrect response for static1/.")
    test_failed = True;
else:
    print("SUCCESS: Static handler replied with the correct response for static1/.")

# Static File handler test: 404 Not Found
# Spawn a shell process to curl to static file handler with nonexistent file path
static404_file_request_command = "curl -i -s localhost:8080/no_file"
static404_file_request_proc = subprocess.Popen(static404_file_request_command, stdout=subprocess.PIPE, shell=True)
static404_response = static404_file_request_proc.stdout.read().decode('utf-8')

expected_static404_response = """HTTP/1.1 404 Not Found\r
Content-Type: text/plain\r
\r
404 File Not Found\n"""

if static404_response != expected_static404_response:
    print("ERROR: Static handler replied with an incorrect response for a 404 request")
    test_failed = True;
else:
    print("SUCCESS: Static handler replied with the correct response for a 404 request.")

# Redirect handler test
# Check if a simple redirect request to an echo request response matches with
# a normal, actual echo request response
run_server_command2 = ["./server", "path_config2"]
server_proc2 = subprocess.Popen(run_server_command2, stdout=subprocess.PIPE)
redirect_request_command = "curl -i -s localhost:8081/redirect"
redirect_request_proc = subprocess.Popen(redirect_request_command, stdout=subprocess.PIPE, shell=True)
redirect_response = redirect_request_proc.stdout.read().decode('utf-8')

expected_redirect_response = "HTTP/1.1 302 Found\r\nLocation: http://localhost:8081/echo\r\n\r\n"

if redirect_response != expected_redirect_response:
    print("ERROR: Redirect handler replied with an incorrect response for redirect/")
    test_failed = True;
else:
    print("SUCCESS: Redirect handler replied with the correct response for redirect/")

# Reverse Proxy handler test
# Run two instances of the web server, with one configured as a proxy
# Make a request to the proxy, which then directs to the backend server
# The response should be the same as backend being requested from directly
proxy_request_command = "curl -i -s localhost:8080/simple_proxy/static/index.html"
proxy_request_proc = subprocess.Popen(proxy_request_command, stdout=subprocess.PIPE, shell=True)
proxy_response = proxy_request_proc.stdout.read().decode('utf-8')
backend_request_command = "curl -i -s localhost:8081/static/index.html"
backend_request_proc = subprocess.Popen(backend_request_command, stdout=subprocess.PIPE, shell=True)
backend_response = backend_request_proc.stdout.read().decode('utf-8')

if proxy_response != backend_response:
    print("ERROR: Proxy handler replied with an incorrect response for simple_proxy/")
    test_failed = True;
else:
    print("SUCCESS: Proxy handler replied with the correct response for simple_proxy/")
    
# Reverse Proxy Redirect test
# Run two instances of the web server, with one configured as a proxy
# Make a request to the proxy, which is a redirect to echo request
# The response should be the same as backend being requested from directly
proxy_redirect_request_command = "curl -i -s localhost:8080/redirect_proxy/"
proxy_redirect_request_proc = subprocess.Popen(proxy_redirect_request_command, stdout=subprocess.PIPE, shell=True)
proxy_redirect_response = proxy_redirect_request_proc.stdout.read().decode('utf-8')

expected_proxy_redirect_response = """HTTP/1.1 200 OK\r
Content-Type: text/plain\r
\r
GET /echo/ HTTP/1.1\r
Host: localhost: 8081\r
User-Agent: curl/7.35.0\r
Accept: */*\r\n\r\n"""

if proxy_redirect_response != expected_proxy_redirect_response:
    print("ERROR: Proxy handler replied with an incorrect response for redirect_proxy/")
    test_failed = True;
else:
    print("SUCCESS: Proxy handler replied with the correct response for redirect_proxy/")

# Terminate server process before exiting
server_proc.terminate()
server_proc2.terminate()

if test_failed:
	print("At least one test case failed -> exiting with exit code 1")
	exit(1)
else:
	print("All test cases passed -> exiting with exit code 0")
	exit(0)
