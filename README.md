# ProxyEmy

Simple and fast local reverse proxy server fit as tool as development.  
All application configuration are stored in a YAML file and can be shared with other teammates (via Git etc).

## Use cases
- Resolve CORS issues in local development, all your websites and services will be on one host.
- Test web applications that require a secure (HTTPS) connection on a local machine (PWA etc).
- Application can be logging external requests and show them on a special page in UI.
- Unification of all your (micro)services on one host inside human-readable aliases or prefixes   
  as example  
  http://localhost/ - for frontend (actually http://localhost:9340/),  
  http://localhost/auth - for authorization service (actually https://192.168.55.20:8000)  
  http://localhost/user - users service (actually http://commonuserservice:12230)  
  and so on
