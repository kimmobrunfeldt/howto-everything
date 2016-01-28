# [CloudFlare](https://www.cloudflare.com/)

![CloudFlare logo](img/cloudflare-logo.png)

[CloudFlare](https://www.cloudflare.com/) is a service which makes your website
faster and increases security.

### Why?

* Faster page loads simply because the responding server is closer to you physically
* If your server dies in the background CDN could still serve cached pages
* To survive DDoS attacks

### CloudFlare Pros

* They have proved themselves, e.g. reddit.com and stackoverflow.com are customers

    [According to them](https://www.cloudflare.com/customers/), >2M websites are
    routed through CloudFlare

* Nice features: CDN, Analytics, DDoS protection

    But be sure to check what plan you need. They have a bit different set of
    [features](https://www.cloudflare.com/plans/).

* [Pricing](https://www.cloudflare.com/plans/) is quite cheap]

    Used bandwidth doesn't matter, it is a fixed price per month.

* They provide an [API](https://api.cloudflare.com/)

### CloudFlare Cons

* Each subdomain is a "new" site.
* They limit the file size of uploads if you are uploading through CloudFlare.

    See limits for each plan: https://www.cloudflare.com/plans/

* Their business model [seems strange](https://www.bizety.com/2014/02/17/cloudflare-business-model/)

    On the other hand they have gotten a lot of funding:
    [2M in 2009](https://www.crunchbase.com/organization/Cloudflare),
    [22M in 2011](http://techcrunch.com/2012/09/03/from-disrupt-runner-up-to-22-million-in-funding-cloudflare-tells-all/),
    [30M in 2013](http://techcrunch.com/2013/12/17/cloudflare-reveals-50m-round-from-union-square-ventures/),
    [110M in 2015](http://techcrunch.com/2015/09/22/cloudflare-locks-down-110m-from-fidelity-microsoft-google-baidu-and-qualcomm/).

* A few reported strange incidents

    I would be careful if the origin server would be very expensive with
    a lot of traffic. So be careful if you are making your S3 bill cheaper
    by adding a free CloudFlare in front. In [one incident](https://news.ycombinator.com/item?id=5214480),
    after a huge amount of traffic(100TB/month) CloudFlare routed all traffic without a warning
    to the origin server and it ended up costing a lot.

    Also see: https://news.ycombinator.com/item?id=4535544

### Other notes / restrictions:

* You have to setup your domain's DNS configuration to point to CloudFlares servers
* Check that the CDN edge servers are actually near the location you would need them to be

    https://www.cloudflare.com/network-map/

### Alternatives

* [AWS CloudFront](https://aws.amazon.com/cloudfront/)
* [Azure CDN](https://azure.microsoft.com/en-us/services/cdn/)
* [Akamai](https://www.akamai.com/us/en/cdn.jsp)
* [MaxCDN])(https://www.maxcdn.com/)
