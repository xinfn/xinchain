#Xinchain -- A Highly Secure and Scalable Decentralized Public Ledger System

Background
==========
In the last 2-3 decades, we have witnessed the thrive of the internet and how 
it affects us in our daily lives. It truly makes the world a better place for all of us. 
However，while we enjoy the convinience that Internet has offered to us, the internet 
itself has become more and more centralized which is the opposite of the original design of Internet 
and inevitably brings more risks. Basic services such as DNS servers, PKI(public key infrastructures) 
are all centralized, internet can be taken offline once these servers are attacked (DDOS etc.,),
 or even worse, users are led to malicious servers. In the meanwhile, over the
last decade, we’ve seen a shift from locally running desktop apps to cloud-based apps
that store user data on remote servers(large corporarions such as Facebook、Google、 Amazon). 
Because of the values of these data, those coporations have become the prime targets of 
hackers. Once they are compromised, severe outcome happens(Yahoo, 2016, 500 million 
user's information leaked). 

Anyway, the Internet, as we currently know, is rife with identity theft, and spam and hacking.  
To build a more secure healthy Internet, we believe a more decentralized Internet is the 
solution and Xinchain: a scalable decentralized public ledger system is our first step to 
achieve this goal.

Xinchain is decentralized public ledger with its innovative consensus algorithm which can 
achieve at least thousands of tps and high availability despite the existence of Byzantine
nodes. Furthermore, it provides basisc services exactly like DNS, PKI without a trust party
but more secure than the traditional one. Moreover, xinchain network provides end to end 
encryption with forward security which can even resist quantum computers attack. Last but 
not least, the Xinchain platform, buit on Xinchain, is embedded with a variety of crypto 
tools such as: sMPC(secure multi-party computation)， CP-ABE（ciphertext policy-Attribute 
Based Encrypyion），IPFS, Full Homorphic Encryption and so on which supports the development
of many kinds of application with stringent privacy and security requirements. 

Certificated Naming System
==========
In the traditional Internet, users judge an entity's validity they connect by a certificate signed by 
a trusted CA. Ergo, a Internet service provider (e.g Google) usually needs to apply a certificate. 
The applier generates a key-pair(public-key、private key) locally and submit an application request. The CA 
verifies applier's identity then signs a certificate using its own private key and issue it
to the applier. The CA includes applier's Public key key(e.g, RSA 2048bit) and some other related
information, such as applier's name, address, certificate expiration time etc. However, since CA is 
centralized, if CA is taken offline by hacking or compromised from inside. The internet will be left
trust-less without certificate. 

By xinchain, we could build a certificated naming system without a trust party. 
Certificate's security、reliability and availability are guaranteed by the underlying
Decentralized Public Ledger System. Any user can submit a transaction to this ledger 
system to bind a public key(can be viewed as identity key, generated from a ECDSA，RSA
or any other public key cryptosystem) to a human readable name and any other information 
required, such as phone number、 email address. This information can be further used as 
DNS service by Xinchain network, i.e. find a user by a readable name rather than a public 
identity key. Users can also unbind or update these information related to their public 
keys by submit a new transaction and a legit signature is provided. 

End to End Encryption with Quantum Resistance 
==========
To avoid traffic monitoring and targeted attacking, we can build an entirely anonymous Xinchain 
network by end to end encryption with forward secrecy: every message is encrypted with a different
message key and the compromise of the current message key can't be used to recover the previous 
message key. Xinchain will use Double ratchet key update mechanism to derive new message key. 
Double ratchet is popular among secret chat apps, such as Signal, Whatsapp, Telegram. 

Another important feature of Xinchain End to End Encryption is Quantum Resistance. Nowadays, more
and more people are beginning to talk about Quantum Computers which use quantum mechanics to 
massively accelerate computation and how it may affect our lives. The research of quantum computers
opens the Gates with new possibilities for many other research areas such as Machine learning, genetic 
engineering, molecular simulation etc. One of the most important influence is public key cryptography, 
according to the Shor's algorithm, all of the popular Public key cryptosystem(RSA, ElGamal, ECDSA and 
so on) will be broken once a powerful quantum computer emerges. Public key cryptography is widely used 
in applications and protocols assuring the confidentiality, authenticity and non-repudiability such as
electronic communications(https) and data storage. So, if the public key cryptosystem is broken, the 
whole internet is insecure.      

Fortunately, there are already some progress in developing quantum resistant public key cryptography, 
mainly are:
+ Lattice-based cryptography
+ Multivariate cryptography
+ Hash-based cryptography
+ Code-based cryptography
+ Supersingular elliptic curve isogeny cryptography
Yet, currently, these algorithms are either insecure or too inefficient compared to ECDSA and RSA to be 
put in applications. Google is the first who put a Lattice based key exchange algorithm called "New Hope" 
based on Lattice Ring-LWE into application(Boring SSL), but it's for experiment purpose and need to be 
used with traditional ECDSA to offset the efficiency issue. There are some other applications in blockchain
 such as HCash, QRL and efficiency is still the big issue.

In xinchain, we will investigate the newest post-quantum NIST proposal and find a most appropriate 
algorithm which is the best tradeoff between security, performance and key size. Then we will integrate
 it with our end to end encryption protocol: The key exchange part to guarantee quantum resistance. We 
 may still want to integrate it with ECDSA in case efficiency is the issue. On the other hand, the 
encryption part we use AES which is already quantum-resistant don't need further change but updating
parameters to meet the security requirement. 


Off-chain storage system
========================
Currently, block-chain technologies has very limited applications since most of them(Bitcoin、Ethereum) mainly 
served as a database of transactions which is very expensive for a regular node to store(more than 200GB for bitcoin)
 and it will keep growing as time passes by.

On the other hand, the number of transactions be contained in a block is very limited since the maximum block size 
is fixed and each transaction takes up space. This situation leads to low tps(e.g. 7tps tops for bitcoin without 
segwit).

Consequently, one intuitive thought to make xinchain more scalable is not using it as storage medium but rather 
a tool to verify correctness. Complex logic should be kept outside of the xinchain. 

Xinchain platform and applications built on xinchain platform will deal with these complex logic. User's 
application data（either encrypted or plaintext）will be stored on the existing cloud storage(Dropbox, AWS,
 Ali etc.) or distributed storage system like IPFS、 BitTorrent、Emule. Unlike traditional internet service
 where users store their data on remote servers, users in Xinchain have absolute control over the access to
 their own data. Applications and other users can gain access to a user's data via a valida access token 
 issued and signed by this user if it's stored on cloud storage or a file hash if it's stored on distributed 
 file system. If the file is encrypted via AES，3DES etc., extra decryption key will be required to access 
 the raw data. As to the policy of how to gain acess control over a user's data and under what circumstance
 the decryption key will be transfered, it's defined by a Xinchain smart contract. 

Confidential Transactions
============
Nowadays, users' privacy in the internet has been one of the most important issues. Some people may like to post 
their daily activities, photos and moods on the internet(e.g. Facebook, twitter), but for the security sake, other
 users may like to keep their identity and activities in private：such as their names, address, shopping records etc. 
 In the traditional internet service, we trust our privacy with third parties, upload our personal info to the remote 
 servers through registration or basic browsing activity. But somehow, we still get constant harassment calls from 
 telemarkters. The remote trusted servers are compromised or our personal info just be sold by the dishonset third 
 parties. Either way, they are pretty common currently.

A comibination of the decentrilization and crypto privacy technology can solve internet's privacy issue. For a start, 
we can see a transaction in bitcoin can pretty much hide sender and receiver's identity(sender and receiver are just 
a string of characters). However, bitcoin doesn't provide true anonymity. To achieve true confidentiality, we can see 
other efforts made in Dash, ZCash, Monero. Dash uses coinjoin to achieve anoymity but also require a trusted third party. 
ZCash uses zksnark which is pretty advanced zero knowledge proof technology but has following problems:
+ required a trusted setup(public CRS) which may leave a trapdoor;
+ process to generate a private transaction is very slow(more than 40s), therefore, private transaction is not enabled 
by default.

In Xinchain, we intend to use Ring-CT which is already used in Monero to ensure confidentiality. Ring-CT uses Ring signature 
developed by CryptoNote and Confidential transactions designed for Bitcoin by Greg Maxwell. It can protect sender and receiver's 
identities as well as transaction amount but has better performance than Zksnark. We will also make further optimization to this 
technology by improving efficiency and reducing transacion size. Some inovative ideas such as Zkstark will also be considered. 


Consensus Mechanism
============

Other Scaling Solutions
============

AI
============

Various Crypto Tools
============

 





Application Case
========

##Introduction
With the development of the Internet, the transmission of data become more and
more convenient, but at the same time, because the traditional Internet is based
on a centralized architecture, all data need to be transferred through a
centralized platform, this centralized model also brings many problems.

One problem is the reliability of service. Because the data are stored on the
central server, once the central server fails or the service provider shuts down
the service, the data stored on these central servers will also be inaccessible.
Even with the emergence of distributed storage technology, it is still difficult
to solve this problem thoroughly. For example, some time ago, a cloud server
provider malfunctioned, resulting in the loss of all data placed on the cloud
server by customers, causing losses of nearly ten million dollars to customers.

Another is data security issue. Because the data are stored on the central
server, it is easy to cause data loss due to the service provider’s mistakes.
For example, if the copyrighted video content produced by individuals is
published on the online video website, they can gain profits. After being
published on the video website and in the process of playing by other users, the
film files are easy to be leaked without authorization, which will result in the
loss of the publisher's profits.

Transaction cost is also a problem. When trading through centralized platforms,
platforms often benefit by drawing a percentage, which is also determined by
platforms. Platforms can adjust the rules at any time, which is very unfair to
both sides of the actual transaction.


##Xinchain Player
Xinchain player is a decentralized film distribution system based on Xinchain where 
Xinchain is served as a decentralized data trading system. Xinchain aims to change 
the way of data distribution and transaction in traditional central architecture 
by using block chain technology. Xinchain player is composed of three parts: 
decentralized storage and distribution system, decentralized retrieval system and
decentralized economic system.

The user's personal data is published to the decentralized storage and
distribution system through encryption. The encryption key is only stored by the
user, and can be decrypted only after authorization, thus ensuring the security
of user data. Other users can't access data without authorization even if they
download it. Moreover, file data is distributed stored in multiple nodes of the
system, which effectively avoids the problem of data inaccessibility caused by
single point failure and ensures the reliability of data access.

Decentralized retrieval system is responsible for retrieving the data users
want. Users can retrieve interested files according to file name or other
attributes. Because it is decentralized, it also ensures the reliability of the
retrieval system, and will not be unable to retrieve because of one machine
failure. After the user retrieves the data he wants, he can decrypt the access
data by purchasing the data and authorizing the copyright party. The money that
the user purchases the data directly transfers to the copyright party's wallet.
Based on block chain technology, file information published by users and
transaction data will be recorded on the chain, which ensures the authenticity
of data and the reliability of transactions.

Xinchain player is a decentralized film distribution system based on Xinchain.
There are only two roles in this system. One is clients, the one is producers.
Xinchain is based on Blockchain technology. When producers make a good movie,
they can publish this movie to the Xinchain system, then clients can distribute
and share this movie with other clients. When a client wants to watch a movie,
this client needs to spend money on the movie, and the movie producer can
receive money directly. Moreover, clients can earn money by sharing and
distributing movies. Blockchain smart contract will perfectly deal with all
transactions without any trust cost which is out the reach of traditional
central servers.

##Economic Model
Publishing a movie includes movie introduction, posters, trailers, movie and
etc. The purpose of trailers is distributing movies which can attract clients.
Watching trailers, sharing movies with friends can earn points. If a client
searches a new movie in Xinchain system, then a new page with posters and
introductions will pop up, and this client can watch movies if this client has
any interests. After watching a trailer, this client can earn points. If client
A wants to watch the whole movie, this client can spend points to watch the
entire movie. If A shares a movie with friends and friends watch this movie,
client A can earn some points.

Except watching movies and sharing videos with friends can earn points,
uploading bandwidth also can make points. When clients watch trailers and
movies, they can share their bandwidth if they want. Other clients can download
movies from them, and providers can earn points. If you can supply more
bandwidth, the more points you can earn.

Xinchain smart contract can deal with all purchasing and earning points between
video publishers and clients. Furthermore, Xinchain will not issue any token in
Xinchain and points will only flow in Xinchain system. When a client wants to
recharge points, Xinchain will generate points in this client account. Xinchain
will use sidechain technology to do value two-way anchor with a public
blockchain. Clients can use digital currency to recharge points in Xinchain,
then clients can use points to do any trading. After trading, clients also can
redeem their points to other digital currencies.

##Price Mechanism
Publishers can decide the movie price. A publisher should set spending points
for this movie when a publisher decides to publish a movie.

When publishers publish a movie, they should recharge points first in order to
do film promotion, then set reward points for watching trailers and sharing
movies with friends. When this reward point is out, clients can get zero reward
point by watching trailer. If publisher set reward points to zero, clients watch
a trailer earn a zero point. When clients search videos, the system will pop up
the video which provides high reward points to clients.

A client upload sharing bandwidth price will dynamically adjust by bandwidth
supply and demand market. If there is not enough bandwidth in the system, the
system will increase bandwidth price gradually which can attract clients to
share their bandwidth. When there is a lot of bandwidth in the system, the
system will decrease bandwidth price gradually. The reason for doing this is we
want to maximize client’s profits. If the system has enough nodes, the supply
and demand will go to a balanced state, in other words, the bandwidth price will
go to a stable state.

##Copyright Protection
We use an asymmetric encryption algorithm to encrypt the movie content. When a
publisher creates movies, then a public-private key pair was created. The
Xinchain system will encrypt contents by using this key. So, publishers can
distribute the file without any risks. And clients spent zero points on
downloading videos. If clients want to watch the whole video, clients should pay
for that. When clients pay for watching a video, a publisher will authorize
clients to decipher the video. A publisher uses proxy re-encryption tech to
authorize clients, and clients will use a public key to generate a re-encryption
private key. Doing this can secure the movie content and private key from
publishers.

##Government Regulation

Xinchain system should protect movie copyright and create a pure, healthy and
green network for clients. Xinchain system will upload publisher information and
video information to the Blockchain which can be traced.

There are some basic rules in protecting movie content copyright and ensuring
legal videos. Publishers should pass the identity verification process.
Moreover, Xinchain system will record MAC address, IP address, hardware ID and
video hash in blockchain which can persist forever. Xinchain system also
requires publisher official information which can verify publisher IP address.
Only the publisher who passes the verify process can publish the movie in
Xinchain system.

If a publisher wants to provide video services, this publisher should make sure
online until not providing video services. If a publisher is offline, clients
cannot play videos. So, if someone publishes an illegal video, government
officers can trace this person by IP address. Publishers may want to delete
illegal files in his/her computers, but officers can charge this person by
records in the blockchain, because the information in blockchain can’t be
deleted and changed.

Xinchain system also will screen shoot the video content in a random time, then
packages images and original video, and uploads this package to the network. If
these images are illegal, the original video is illegal too. Xinchain will
adjust the shooting frequency. We will make sure the screen images are enough
for forensics and will not leak too much video content.

Xinchain system will provide a reporting entrance. Anybody can report illegal
and pirating videos. If we verify the reporting information, we will lock this
video to the blacklist. So, nobody can share and watch this video in the
network.

##Technology Details


Core business process
---------------------

![](media/5845dcc81b388e617887642d5c9340a1.png)

Core business process image

Movie publishers can publish movies, set video price, trailer price, promotion
price and etc. Xinchain system will encrypt video and relative materials, then
upload these resources to the P2P network and blockchain system.

When clients buy a video, the smart contract will retrieve the video price and
transfer the points to publisher account from clients account. Xinchain system
will record the buying order in blockchain at the same time.

Clients can ask for authorization from the publisher, then clients can play this
video with a proxy-private key.

Video Storage and Video distribution
------------------------------------

Xinchain system based on IPFS which can store video and distribute video. IPFS
is a P2P distributed file system. It aims to replace HTTP and build a faster,
secure, free internet.

According to the Xinchain business model, we make a lot of improvement in IPFS.
XPFS will segment videos into small pieces, and each piece video file contains
some backup video files. So, in the upload bandwidth incentive mechanism, the
number of pieces of video will dynamically adjust by hot video or not. When
clients download videos, clients will download video from nearest and best
nodes. If more and more people watching a video, clients can find this video at
any node, so clients can get a better experience.

Video Encryption and Authorization
----------------------------------

We use Proxy re-encryption to encrypt video and authorize the client. Proxy
re-encryption will keep the publish private key safe.

Proxy re-encryption is a public key password, and there is a proxy agent in this
system. In this system, an agent can get re-encryption private key, then use
this private key to decode the file. Proxy re-encryption can make sure agent can
get nothing about plain text through this agent gets a private key.

There is a very important standard in Proxy re-encryption, called anti-collusion
attach. The anti-collusion attack is that agent and authorized people can’t get
the real private key when they come together.

Xinchain system is a decentralized system, so there are only two roles in this
system. One is clients, the other one is publishers. Xinchain uses proxy
re-encryption technology and combines the agent with the client side. Then use
client private key to decode encrypted video data. Anti-collusion makes sure
client and agent can’t get a private key though they are together.

![](media/86fc880a1631152aa5be7b783e6635b8.png)

Video encryption and authorization process

Smart contract
--------------

We will record all user data, publishing video history and purchasing history.
This system is based on EOS blockchain tech. And we build a smart contract to
maintain data.

The smart contact includes the following features:

### Create an Account

We will create a ringleaieos account in the blockchain system, then use this
account to invoke “create account” function in smart contract to create a
xinchain account. We will initial all settings to the default state. Xinchain
accounts are one-to-one correspond to the blockchain accounts. Ringleaieos
account will record account balance of points, usage of points, incomes, and
files information.

### Publish Videos

Video publisher can invoke “publish” function in the smart contract. The
Xinchain player will transfer a video hash address and a video price to the
smart contract. This smart contract will check the hash address first. If the
smart contract has this hash address, the smart contract will terminate this
process and return an error to Xinchain player. If this hash code is new to the
smart contract, this contract will relate video info with a video publisher.
This information will be upload to the blockchain.

### Buy Videos

Clients can invoke “buyvideo” function, then Xinchain player transfers a hash
address to the smart contract. The smart contract will check this client already
buy a movie or not. If someone already buys this movie, the smart contract will
reject this request and return an error to Xinchain player. If a client buys
this video in the first time, the smart contract will retrieve video info by
hash address, get publisher account info and video price. If a client has enough
balance, then smart contract transfers points from client account to publisher
account. If this client has not enough balance, the smart contract will
terminate this request.

### Recharge points

Xinchain will support multiple digital currencies. When a client recharges XIN
points, Xinchain client side will send this request to network gate. Then
Xinchain client side will broadcast this transaction to the target blockchain.
This network gate is managed by a famous exchange market and a miner company.
When the gate confirms this transaction, the gate will do the exchange rate
calculation, then the gate will send XIN points to a client’s address. This
network gate is a decentralization gate. XIN is a single anchor to USD.
Normally, 1 XIN equal 1 dollar.

### Redeem points

If a client wants to redeem points, this client just inputs the number of Xin
points and target blockchain address, then the Xinchain client side will deal
with all transaction. If a client clicks “redeem button”, Xinchain player will
broadcast this transaction to the target blockchain multiple sign address. When
the gate observes this transaction, it will transfer digital currency to the
correspondence account.

##Future Planning

Xinchain player allows every creative content producer to release their original
videos, make money directly through authorized playback, and let every content
producer have a platform for self-realization. Although xinchain player's goal
is to change the existing film distribution model, it is not mutually exclusive
for traditional video platforms. They have a lot of copyrighted film, including
purchased and self-made. They can gain profits by accessing xinchain, just like
individual producers, and form a good complement with their own advantages,
which together constitute the xinchain video distribution ecosystem.

From Xinchain's model, we can see that the core elements are the decentralized
storage and distribution system, the decentralized retrieval system and the
decentralized economic system. Xinchain player is only one of the applications
based on Xinchain. Besides the video files, other data is also feasible, such
as, in the field of health care, everyone's genetic data and health data and so
on. The ownership of these data belongs to individuals, not to any medical or
detection institutions. If other institutions need to use them, they must be
authorized and paid to individuals. Xinchain can solve this problem well by
replacing the video player with a gene and health data parser. So in the future,
xinchain plans to support more different types of data and build a universal
personal ownership data trading system.
