import subprocess

run_server_command = "./server example_config"
server_proc = subprocess.Popen(run_server_command, stdout=subprocess.PIPE, shell=True)

run_curl_command = "curl -i localhost:8080"
curl_proc = subprocess.Popen(run_curl_command, stdout=subprocess.PIPE, shell=True)
print(curl_proc.stdout.read())

server_proc.terminate()