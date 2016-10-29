# Heroku rocks

I really like Heroku. To my knowledge, they are the only cloud provider which actually 
delivers the promise: "Focus to your apps instead of infrastructure".

I think using Heroku is the defacto way to go in most cases, especially when validating new concepts.
There's no time to waste in server setup.

## Good

* Deployment automation
* Versioning
* Cloud practices: forces you to separate stateless web apps from DBs and file storage layers
* A bit same as the previous but, they actually follow their own very good rules for cloud services http://12factor.net/
* Schedulers are easy
* Worker architecture is easy
* Tooling
* Documentation
* Heroku is the thought-leader of cloud providers:  interagent/http-api-design: HTTP API design guide extracted from work on the Heroku Platform API on GitHub
* You can always use parts of AWS/Google cloud aside with Heroku apps

## Not so good

* Docker support
* Price. But remember to give some value for your time too. Heroku saves a lot of time.
* For MS techs
* Multiregion superhigh-availability
* Static IP addresses. Causes trouble in apex domain DNS setup or integrations to systems which need firewall whitelisting
  
  Private Spaces help with outbound static IP ranges


## Other notes

* Don't use free plans for crucial cloud parts in production apps. E.g. dynos, databases, redis, message queue should all be in paid plans.
