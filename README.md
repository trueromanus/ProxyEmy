# ProxyEmy

Simple and fast local reverse proxy server fit as tool as development. The application is fully native, you don't need to install additional stuff. All application configuration are stored in a YAML file and can be shared with other teammates (via Git etc). You can also change the configuration in a simple UI. Currently not supported h2/h3.  
![Image woth screenshoot of application](https://github.com/trueromanus/ProxyEmy/blob/main/image.png?raw=true)

## Use cases
- Resolve CORS issues in local development, all your websites and services will be on one host.
- Test web applications that require a secure (HTTPS) connection on a local machine (PWA, Android emulator etc).
- Application can be logging external requests and show them on a special page in UI.
- Demystification HTTPS traffic for sniffers.
- Unification of all your (micro)services on one host inside human-readable aliases or prefixes   
  as example  
  http://localhost/ - for frontend (actually http://localhost:9340/),  
  http://localhost/auth - for authorization service (actually https://192.168.55.20:8000)  
  http://localhost/user - users service (actually http://commonuserservice:12230)  
  and so on
## Example of configuration file
```yaml
port: 8090  # Specify port for proxy server. Default is 8080.
secure: true  # This option enabling (if value is equal true) secure HTTPS connection. Default is false.
logrequests: false  # You can logging routes if value is equal true. Default is false.
verifyCertificates: true  # You can enable strong check (if value is equal true) for certificates while SSL handshake. Default is false.

# Section for specifying aliases, if you have repetitive paths it will be better to specify it as an alias.
# format for value is `<alias name> <original value>`
aliases:  
  - innerhost http://192.168.3.2 
  - outerhost https://outerhost.com

# Section for specifying mapping between inner route and the external route. 
# The external route can be any valid address, as internal also as external from the web.
# format for value is `<inner route, must be start with slash> <full path for proxy, you can using aliases>`
# for using alias inside mapping value you need using format {<name of alias>}.
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
