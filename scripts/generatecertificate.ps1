$PathToOpenSSL = $args[0]
$PathToOpenSSLBinary = $args[0] + "\Win_x64\bin\openssl.exe"

$env:OPENSSL_CONF = $PathToOpenSSL + "\openssl.cnf"

&$PathToOpenSSLBinary req -x509 -nodes -new -sha256 -days 1024 -newkey rsa:2048 -keyout RootCA.key -out RootCA.pem -subj "/C=US/CN=ProxyEmy-Root-CA"
&$PathToOpenSSLBinary x509 -outform pem -in RootCA.pem -out RootCA.crt

&$PathToOpenSSLBinary req -new -nodes -newkey rsa:2048 -keyout localhost.key -out localhost.csr -subj "/C=US/ST=YourState/L=YourCity/O=ProxyEmy-Certificates/CN=localhost.local"
&$PathToOpenSSLBinary x509 -req -sha256 -days 1024 -in localhost.csr -CA RootCA.pem -CAkey RootCA.key -CAcreateserial -extfile domains.ext -out localhost.crt
