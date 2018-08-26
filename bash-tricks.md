### Find files containing text recursively from current directory

    egrep -lir --include=*.txt "text inside file" .

### List tar(.gz) file contents

    tar -tvf file.tar
    tar -tzvf file.tar.gz

### Monitor network in/out traffic with slurm

    sudo apt-get install slurm
    slurm -i eth0

### Bench mark HTTP server with Apache Bench(ab)

    # Bench mark with HTTP basic auth, custom header, 10 concurrent connections, 1000 times. Use IP instead of localhost
    ab -r -v 4 -A user:pass -H "X-Header: test" -c 10 -n 1000 http://127.0.0.1:3000/status/

### Replace all tabs to 4 spaces with expand. Finds all .js files in the current dir and its subdirs

    find . -name '*.js' ! -type d -exec bash -c 'expand -t 4 "$0" > /tmp/e && mv /tmp/e "$0"' {} \;
    
### Remove node_modules recursively

    find . -name "node_modules" -type d -prune -exec rm -rf '{}' +

### Count lines of code recursively from dir

    find src -name '*.js' -exec wc -l {} +

### Loop

    #!/bin/bash

    while true; do
        ssh "$@"
        sleep 1
    done

### Print all Heroku router requests which took over 1000ms

    # On OSX: brew install gawk
    gawk '(match($0, /service=(.*)ms/, arr)) { if (arr[1] > 1000 ) { print $0 } }' <LOGFILE>

* `$0` means the full line
* `arr[1]` is the matched regex group


### Parse XML response with sed

Assuming the XML api returns `<TagX>something</TagX>`, the command would output `something`.

```
curl "https://xmlapi.service.com" | sed -n 's:.*<TagX>\(.*\)</TagX>.*:\1:p')

# or
echo '<xml><TagX>  something</TagX></xml>' | sed -n 's:.*<TagX>\(.*\)</TagX>.*:\1:p'
```
