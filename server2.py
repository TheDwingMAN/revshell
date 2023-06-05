
from http.server import BaseHTTPRequestHandler
from socketserver import TCPServer
from threading import Thread, Event, Semaphore
import os
import queue
import urllib


class ServerOpcode:
    close,powershell,donwloadfiles = 1,2,3

class WebServer:
    def __init__(self) -> None:
        pass
    def start() ->None:
        pass
    def shutdown()-> None:
        pass
    def run()->None:
        pass


def main()->None:
   print( ServerOpcode.close)


if __name__ == "__main__":
    main()