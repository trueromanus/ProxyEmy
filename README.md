# ProxyEmy

Simple and fast local reverse proxy server fit as tool as development.  
All application configuration are stored in a YAML file and can be shared with other teammates (via Git etc).

## Use cases
- Resolve CORS issues in local development, all your services will be on one host
- Unification of all your (micro)services on one host inside human-readable aliases or prefixes (http://localhost/ - for frontend, http://localhost/auth - authorization service, http://localhost/user - users service and so on)
