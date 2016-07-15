# Google Cloud Container Engine - First impressions

> Run Docker containers on Google Cloud Platform, powered by Kubernetes.

What they promise:

* Easy container management and setup
* Run docker containers in cloud
* JSON to configure CPU, memory etc.
* Logging
* Reserved IP ranges
* Free for up to 5 nodes
* Open source tools


Sounds good, let's [get started](https://cloud.google.com/container-engine/docs/before-you-begin)
by creating a new project, but first ..
what's [Kubernetes](http://kubernetes.io/)?

> Manage a cluster of Linux containers as a single system to accelerate Dev and simplify Ops.

Ok, an open source thingy.. woah 24 527 commits by 650 contributors. Must be a serious thing.

Continuing with the setup, account billing and Container Engine API need to be
enabled before using it. Let's create a new project called `busse-api`.
That project is currently running on Heroku, so I can easily compare them.
Google let's you to choose
the server node location from us-central, us-east1 or europe-west.
I'll choose the europe-west one.

After this the mangement UI says "it takes a few minutes to get the Container Engine ready".
In the meanwhile, let's install the needed tools. They provide a basic
"pipe content from our site to bash" -installation, which I will happily run.
It added a few things to my .zshrc, after a confirmation.

Running `gcloud init` pops up a browser tab, where I can give Google Cloud Platform
a bunch of rights when logging in with my Google credentials. It redirects that back
to localhost, so apparently gcloud command setup a temporary local http server
for the trick, neat. It then asks you for a few default settings.

The kubectl can be installed with `gcloud components install kubectl`. Now the
management UI has done its background work, let's create a container cluster.
Let's name it `busse-api` too. I choose two of the laziest micro machines
to europe-west-b1 zone. Error, you need to have 3 machines in the cluster
with the chosen machine type. No possibility to edit - delete and back. It was
really fast, so no problem. Second try. It takes a while to create the cluster.

After it's done, we can configure it to command line:

```bash
gcloud config set container/cluster busse-api
# It matters what you set in the compute zone in the previous gloud init command
gcloud container clusters get-credentials busse-api
```

Ok now the cluster of computers should be running in Google Cloud. Now I'm at the
same point what Heroku provides me, almost. Next, [deploying the Node project](https://cloud.google.com/container-engine/docs/tutorials/hello-node)
to the cluster.

```
cd busse-api
export PROJECT_ID=your-project-id-here
```

Create Dockerfile. They are advising quite old version of node, let's define our own
based on [nodejs.org guide](https://nodejs.org/en/docs/guides/nodejs-docker-webapp/).

```
FROM node:argon

# Create app directory
RUN mkdir -p /usr/src/app
WORKDIR /usr/src/app

# Install app dependencies
COPY package.json /usr/src/app/
RUN npm install

# Bundle app source
COPY . /usr/src/app

PORT=8080
# Still using Babel 5 in this project..
CMD [ "./node_modules/.bin/babel-node", "./src/index.js" ]
```

Push that to the Google Container Repository:

```
docker build -t gcr.io/${PROJECT_ID}/busse-api:v1 .
gcloud docker push gcr.io/${PROJECT_ID}/busse-api:v1
```

*While running the second command, it returned 500 Internal Server Error once*

Create a [pod](https://cloud.google.com/container-engine/docs/pods/):
```
kubectl run busse-api --image=gcr.io/${PROJECT_ID}/busse-api:v1 --port=8080
```

At this point I got somd build errors first time because of a problem in my
Dockerfile. I hadn't told it to run npm install. Checking the error logs
was quite a challenge as I wasn't sure how to do it. Fortunately Google has
a [debugging pods section](https://cloud.google.com/container-engine/docs/debugging/#debugging_pods)
in their docs. After 5 minutes of trying different commands, I got some logs
showing. In general, this shows what is happening in your pod: `kubectl describe pods`.
In the end I got the original error with this command:
`kubectl logs --tail=100 --previous busse-api-1c2dl busse-api`. Where `busse-api-1c2dl`
is the Name when running describe pods.

After the Dockerfile fixes and repushing the image to Google container repo,
I got my app running, almost! Describe pods says "Back-off restarting
failed docker container". Ok, so let's check the logs from the container:

```
Error: MTA_API_KEY variable is not set!
    at Object.<anonymous> (/usr/src/app/src/init-env-variables.js:11:11)
    at Module._compile (module.js:410:26)
    ...
```

It's at least running my code already but now I need to specify the environment
variables. This leads us to [Kubernetes Secrets documentation](http://kubernetes.io/v1.1/docs/user-guide/secrets.html#secrets).



**THIS IS WHERE I LOST MY INTEREST..**

> The command-line tool (kubectl) required for Google Container Engine is
> supported on Linux and Mac OS X. Windows users can start a Google Compute
> Engine instance to access a Linux environment from which to run the tool.
