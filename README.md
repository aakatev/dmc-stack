# cpp-web

Example of a website running c++ on client and server side

**Build with:**

* [Crow](https://github.com/ipkn/crow) on backend
* WebAssembly modules on frontend

**Docker Mini-Tutorial**


- Running this container

```
docker run -ti cpp-server:latest bash
```


- Running bash in container with mounted volume

```
docker run -v /home/aakatev/Desktop/git/cppweb:/usr/src/cppweb -ti cpp-server:latest bash
``` 


- Running server from a container with mounted volume and port 8080 exposed

```
docker run -v /home/aakatev/Desktop/git/cppweb:/usr/src/cppweb -p 8080:8080 -e PORT=8080 -ti cpp-server:latest /usr/src/cppweb/server-source/build/server-source
```