*Note: this discussion was Google-translated.*


> How to configure wildcard HTTPS *.subdomain.domain.com to Heroku?
- [@mironieminen](https://twitter.com/mironieminen)


> Heroku ACM does not support LE wildcard certs, so if you want to use Heroku, you will have to apply 
> the cert yourself, for example as a cronjob. As such, short scripts can be found on Google, so no 
> problem for Mr. manual labor. But LE wildcart cert requires DNS Challenge, so you need API Access for 
> your DNS provider. If you have static thingery, swap it with Netlify, with DNS there, and ask support to 
> activate support for the wildcard subdomain. Success with the pro plan.
> 
> The third option is to use some Heroku SSL plugin to get wildcard ssl 
> (instead of Heroku's own ACM). 
> In those, I think the cheapest was something like 75 a month.
- [@JouniKaplas](https://twitter.com/JouniKaplas)

