# Load testing with `ab`

**Beware:** for real load tests you should use an external service which
generates actual load from different IP addresses etc. This testing gives
you some idea of the load handling capabilities of your web app.

For this quick test, we'll be using a tool called `ab`.

`ab` is a small tool by Apache for generating concurrent requests to a HTTP url.
Documentation: http://httpd.apache.org/docs/2.4/programs/ab.html

Install on OS X: `brew install ab`

## Good response times

Everything depends on how fast / performant you want your API to be, but
I would roughly say that a fast, high available API should fill these requirements:

* Median response time should be <50ms
* 95th percentile response time should be <100ms (95% requests handled under this repsonse time)
* Handle at least ~1000 requests per minute (17 req/s).

  If your API can't handle that much, you should do caching or other measures
  to enable proper performance.

  1000 req/min is the minumum, but you should aim for
  handling at least ~5000 request per minute.


## Running a test

Let's execute a basic test which sends a lot of GET requests to a single
HTTP url.

`ab -vv -k -n 1000 -c 50  http://app.herokuapp.com/api/v1/some/resource`

* `-vv` Verbose
* `-k` Enable the HTTP KeepAlive feature, i.e., perform multiple requests within one HTTP session. Default is no KeepAlive. **Enable it because browsers use it too**
* `-c 50` Number of multiple requests to perform at a time. Default is one request at a time.
* `-n 1000` Number of requests to perform for the benchmarking session. The default is to just perform a single request which usually leads to non-representative benchmarking results.

Then you get this: `Requests per second:    110.63 [#/sec] (mean)`
and it will report what the respond times were, you should aim them to be under 100ms


### Is my app able to handle 20 000 monthly users?

You need to have some kind of estimate what the X amount of users in month means
as requests per minute or requests per second.

So for example, if you have 20k users per month. You should figure out
the max peak of requests at any given minute. If you have Google Analytics,
check the hourly page views for the last month / year. Find the busiest hour
from the analytics history. Come up with a pessimistic estimate what could
be the largest amount of requests per minute. E.g. if in 1 hour you have
had 5k page views, it would mean 83 req/min - but better make sure you can handle
500 req/min to handle peaks. Everything depends how large of a traffic burst peaks
you have.

