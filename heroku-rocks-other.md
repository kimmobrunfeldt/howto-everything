They are the only cloud provider which understands and
delivers the promise: "Focus to your apps instead of infrastructure".





## Good

* Deployment automation

  `dev -> qa -> prod` deployment pipeline is built into Heroku.

  Takes 5 minutes to configure setup where master branch is automatically
  deployed to Heroku if CI tests pass.

* Very low vendor lock in from technical point of view

    Application code itself has none or very little knowledge that it
    is being run at Heroku. Heroku forces you to write application code in
    a way that it will be portable to any other hosting provider.

    Mental lock in is big because you don't want to deal manually
    with the stuff Heroku takes care for you.

* Versioning
* Rollbacking application code and Postgres database is easy

    Provides a safe environment for rapid deployment cycles. You can always
    rollback to previous version without stress.

* Postgres Dataclips at their best can replace simple admin UI
* Cloud practices: forces you to separate stateless web apps from DBs and file storage layers
* A bit same as the previous but, they actually follow their own very good rules for cloud services http://12factor.net/
* Schedulers (cron) are easy to setup
* Worker architecture is easy to setup and maintain
* Commandline tooling
* Documentation
* Heroku is the thought-leader of cloud providers:  interagent/http-api-design: HTTP API design guide extracted from work on the Heroku Platform API on GitHub
* You can always use parts of AWS/Google cloud aside with Heroku apps
* Logging is handled well by the platform

    Tracing errors from production apps is easy via logs.

* Built-in metrics
* Built-in way to handle secrets (environment variables)
* Built-in way to setup alerts
* Scaling horizontally is easy
* Language runtime can be configured with buildpacks

  -> Heroku supports *any* language which you can compile and setup on Linux.

  Example: https://github.com/mfine/heroku-buildpack-stack

* Heroku teams allow simple way to give access to applications

    It's not even nearly as flexible as IAM users/groups in AWS but covers 95%
    of development needs.


## Not so good

* Direct server and add-on costs. Others solutions may still end up begin more expensive if you take maintenance and development time into account
* For MS techs
* Multiregion superhigh availability
* Static IP addresses. Causes trouble in apex domain DNS setup or integrations to systems which need firewall whitelisting

  Private Spaces help with outbound static IP ranges

* Customizing infrastructure, VPN tunnels etc

## Other notes

* Don't use free plans for crucial cloud parts in production apps. E.g. dynos, databases, redis, message queue should all be in paid plans.


## Problems with AWS Beanstalk

* AWS gives you all options, but doesn't force towards good conventions

    https://wblinks.com/notes/aws-tips-i-wish-id-known-before-i-started/

    Developers need to learn and know **a lot** of things. When there's
    too many steps to setup a secure, high available hosting environment,
    corners are being cut.

* You are responsible for patching OS vulnerabilities
* Developer experience isn't that good

    In practice this means more hours of debugging infrastructure issues
    which doesn't create any value.

* Centralized logging setup isn't easy to setup

  http://stackoverflow.com/questions/27324582/is-it-possible-to-have-centralised-logging-for-elasticbeanstalk-docker-apps

* Patching OS vulnerabilities isn't easy

  http://serverfault.com/questions/495163/how-to-upgrade-to-the-latest-ami-in-aws-elastic-beanstalk?noredirect=1&lq=1
  http://serverfault.com/questions/518824/automatically-applying-security-updates-for-aws-elastic-beanstalk

## PaaS vs SaaS

http://stackoverflow.com/questions/9802259/why-do-people-use-heroku-when-aws-is-present-whats-distinguishing-about-heroku
