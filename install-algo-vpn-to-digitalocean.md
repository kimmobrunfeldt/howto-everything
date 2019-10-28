# Install Algo VPN to Digital Ocean (from MacOS)

[Algo](https://github.com/trailofbits/algo) is an easy way to set up a personal VPN in the cloud. 



## Install server

In case something is missing here, refer to their DO install guide at https://github.com/trailofbits/algo/blob/88eaf30e65df5ac2128f035954b275608f380fc1/docs/cloud-do.md.


Generate an API token in Digital Ocean, then run:

```
export DO_TOKEN=<your-token-herer>
brew install ansible
git clone git@github.com:trailofbits/algo.git
cd algo

python3 -m pip install -U -r requirements.txt

ansible-playbook main.yml -e "provider=digitalocean
                                server_name=algo
                                ondemand_cellular=false
                                ondemand_wifi=true
                                dns_adblocking=false
                                ssh_tunneling=false
                                store_pki=true
                                region=fra1
                                do_token=$DO_TOKEN"
```

The installation takes quite a while. After it's finished, you should see the following banner. 

```
ok: [x.x.x.x] => {
    "msg": [
        [
            "\"#                          Congratulations!                            #\"",
            "\"#                     Your Algo server is running.                     #\"",
            "\"#    Config files and certificates are in the ./configs/ directory.    #\"",
            "\"#              Go to https://whoer.net/ after connecting               #\"",
            "\"#        and ensure that all your traffic passes through the VPN.      #\"",
            "\"#                     Local DNS resolver X.X.X.X, fdxx::x:xxxx                   #\"",
            ""
        ],
        "    \"#        The p12 and SSH keys password for new users is xxxxxxxx       #\"\n",
        "    \"#        The CA key password is xxxxxxxxxxx       #\"\n",
        "    \"#      Shell access: ssh -i configs/algo.pem root@x.x.x.x        #\"\n"
    ]
}
```

The default configuration is taken from `config.cfg` file, and contains three users: 

* desktop
* laptop 
* phone

You can optionally save the configs to e.g. 1password. `zip -r algo-configs.zip configs` and save the zip file to 1password. If you lose the keys, you can always reinstall the server.


## Install clients

*Refer to https://github.com/trailofbits/algo#configure-the-vpn-clients for full client guides.*



### MacOS

*WireGuard is an open source VPN client, which makes installing the client VPN configs easier. See https://www.wireguard.com/.*   

Install WireGuard at https://apps.apple.com/us/app/wireguard/id1451685025?mt=12 and open the app. Click "install via configuration file" or similar and browse to `configs/x.x.x.x/wireguard/laptop.conf`.
    
### iOS

Install WireGuard at https://itunes.apple.com/us/app/wireguard/id1441195209?mt=8, and scan the QR code from your computer. 

To show the QR code, run *(where x.x.x.x is the IP address of the created Droplet)*:

```
open configs/x.x.x.x/wireguard/phone.png
```    

## Glossary

* **On demand** = automatic VPN connection
