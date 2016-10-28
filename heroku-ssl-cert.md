# Heroku SSL setup for custom comain

Guide for dummies like me how to setup Heroku SSL for your custom domain.

This guide will use the Heroku SSL for paid dynos ( https://devcenter.heroku.com/articles/ssl ).
[Heroku announced](https://blog.heroku.com/ssl-is-now-included-on-all-paid-dynos)
this new "free" SSL for all paid dynos on September 22nd, 2016.


## Ingredients

You will need:

1. Server SSL certificate (e.g. .crt, .pem) **Referred as "server cert(ificate)" later**

  Wildcard certificate `*.yourdomain.com` is the easiest.

  This file is a single certificate. Should contain only one
  `-----BEGIN CERTIFICATE-----`.

  Example file name: `ServerCertificate.crt`

2. Server private key (.key) **Referred as "private key" later**

  Should contain one `-----BEGIN RSA PRIVATE KEY-----`.

  Example file name: `your_domain.key`

3. Intermediate certificate or certificate chain (e.g. .crt, .pem) **Referred as "chain certificate" later**

  Should contain multiple `-----BEGIN CERTIFICATE-----`.

  Example file name: `ChainBundle.crt`

4. Ability to change your domain DNS records

  If you want your customers to be able to use yourdomain.com directly,
  **double check** that you have DNS provider which supports ALIAS/ANAME or similar
  CNAME-like functionality for apex domain *(yourdomain.com without subdomain)*.

  See: https://devcenter.heroku.com/articles/custom-domains#add-a-custom-root-domain


**Note**: 1. and 3. may be bundled together already. This is usually called something like certificate bundle.


#### .pem, .crt, .what??

Certificate may be in different formats: https://en.wikipedia.org/wiki/X.509#Certificate_filename_extensions

Easiest to work with is base64 encoded (.pem format). If you see
`-----BEGIN CERTIFICATE-----` and bunch of ascii strings, it's base64 encoded
certificate (.pem format).

With the binary formats, you'll need `man openssl` to manipulate them


## Setup

*These steps can be done with Heroku CLI too*

1. Go to app settings, "Domains and certificates" section
2. Add your domains to Heroku app settings

  For all wanted domains, click Add domain, and input your domain.
  For example I want to add:

  * `www.yourdomain.com`
  * `yourdomain.com`

  After adding, forget what Heroku says in the DNS Target column for now. It
  will change after we upload the SSL cert.

3. Click "Configure SSL"
4. Combine server certificate and chain certificate as a one certificate bundle. (or use existing bundle)

  To combine, run:

  ```
  cat ServerCertificate.crt ChainBundle.crt > bundle.crt
  ```

  The order of files matter. The server certificate needs to
  be the first, and chain cert second. Otherwise Heroku will warn that the private
  key does not match the public cert.

  Then validate the generated bundle:

  ```
  openssl verify -verbose -purpose sslserver -CAfile ChainBundle.crt bundle.crt
  ```

  If it says `bundle.crt: OK`, it's formatted correctly.

5. Upload bundle.crt to Heroku
6. Add `.key` file to Heroku
7. Configure your DNS records

  For each domain you configured to Heroku previously, add these records
  to `yourdomain.com` domain:

  Record  | Subdomain   | Target
  --------|-------------|------------
  CNAME   | www         |  www.yourdomain.com.herokudns.com
  ALIAS   | @ or empty  |  yourdomain.com.herokudns.com

  The apex domain is marked here as `ALIAS`, it differs across DNS providers.
  E.g. In Cloudflare you can just use CNAME even in the apex as they are
  doing CNAME flattening.

  Read more: https://devcenter.heroku.com/articles/custom-domains#add-a-custom-root-domain

8. Wait until your DNS changes have been propagated. TTL of the records may be from seconds to hours.
9. Very that a SSL certificate checker says that the certificate is correctly installed

  E.g. https://www.digicert.com/help/, just write your domain to their checker.


## Problems I bumped into

#### Android browser: "Your connection is not safe"

Android gave the red lock of death after I though the certificates were
correctly installed. However I had only installed the ServerCertificate.crt
to Heroku. Fix was to combine server certificate + chain cert and upload the
bundle.crt to Heroku.

#### Heroku says private key doesn't match to certificate

Server certificate needs to be the first certificate in the `bundle.crt`.



