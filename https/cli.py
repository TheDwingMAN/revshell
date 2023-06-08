import http.client
import ssl


def send_get_request(host, port, path):
    context = ssl.create_default_context()
    context.check_hostname = False
    context.verify_mode = ssl.CERT_NONE

    conn = http.client.HTTPSConnection(host, port, context=context)

    conn.request('GET', path)
    response = conn.getresponse()
    print(f'Response: {response.read().decode()}')

    conn.close()


def send_post_request(host, port, path, data):
    context = ssl.create_default_context()
    context.check_hostname = False
    context.verify_mode = ssl.CERT_NONE

    conn = http.client.HTTPSConnection(host, port, context=context)

    headers = {'Content-type': 'application/x-www-form-urlencoded'}
    conn.request('POST', path, body=data, headers=headers)
    response = conn.getresponse()
    print(f'Response: {response.read().decode()}')

    conn.close()


if __name__ == '__main__':
    HOST = 'localhost'
    PORT = 8443

    # Send GET request
    send_get_request(HOST, PORT, '/')

    # Send POST request
    send_post_request(HOST, PORT, '/', 'data=Hello, HTTPS Server!')
