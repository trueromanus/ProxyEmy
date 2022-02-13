# ProxyEmy

Simple and fast local reverse proxy server fit as tool as development.  
All application configuration are stored in a YAML file and can be shared with other teammates (via Git etc).  
You can also change the configuration in a simple UI.  
Currently not supported h2/h3 and WS/WSS.

## Use cases
- Resolve CORS issues in local development, all your websites and services will be on one host.
- Test web applications that require a secure (HTTPS) connection on a local machine (PWA, Android emulator etc).
- Application can be logging external requests and show them on a special page in UI.
- Unification of all your (micro)services on one host inside human-readable aliases or prefixes   
  as example  
  http://localhost/ - for frontend (actually http://localhost:9340/),  
  http://localhost/auth - for authorization service (actually https://192.168.55.20:8000)  
  http://localhost/user - users service (actually http://commonuserservice:12230)  
  and so on
## Example of configuration file
```yaml
port: 8090
secure: true
logrequests: false

aliases:
  - innerhost http://192.168.3.2
  - outerhost https://outerhost.com
  
mappings:
  - / {innerhost}
  - /auth {innerhost}:8080/api/auth
  - /processing {innerhost}:9000
  - /ticketintegration {outerhost}/tickets
```
## Supported platforms
- Windows 10+
- Linux (partually support)
- macOS 10.14+ (partually support)

## How to build
### Requirements
- CMake >= 3.14
- Qt >= 6.2.1
### Commands
```
cd src
cmake --build
cmake --install
```
