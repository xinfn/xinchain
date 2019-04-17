Xinchain -- A Highly Secure and Scalable Decentralized Public Ledger System
==========

Introduction
==========
In the last 2-3 decades, we have witnessed the thrive of the internet and how 
it affects us in our daily lives: it truly makes the world a better place. 
However，while we enjoy the convinience that Internet has offered to us, the internet 
itself has become more and more centralized which is the opposite of its original design
and inevitably brings more risks. Basic services such as DNS servers, PKI(public key infrastructures) 
are all centralized, internet can be taken offline once these servers are attacked (DDOS etc.,),
 or even worse, users are led to malicious servers. In the meanwhile, over the
last decade, we’ve seen a shift from locally running desktop apps to cloud-based apps
that store user data on remote servers(large corporarions such as Facebook、Google、 Amazon). 
Because of the values of these data, those coporations have become the prime targets of 
hackers. Once they are compromised, severe outcome happens(Yahoo, 2016, 500 million 
user's information leaked). 

Anyway, the Internet, as we currently know, is rife with identity theft、spam and hacking.  
To build a more secure healthy Internet, we believe a more decentralized Internet is the 
solution and Xinchain: a scalable decentralized public ledger system is our first step to 
achieve this goal.

Firstly, Xinchain is a decentralized public ledger with its innovative consensus algorithm which can 
achieve at least thousands of tps and high availability despite the existence of Byzantine
nodes. Secondly, it provides basisc services exactly like DNS, PKI without a trust party
but more secure than the traditional one. Thirdly, xinchain network provides end to end 
encryption with forward security which can even resist quantum computers attack. Fourthly, Xinchain’s 
offchain distributed storage system will make it support more application cases. Fifthly, it supports 
confidential transaction to hide sender、receiver's identity and other transaction details.
Plus, the Xinchain platform, buit on Xinchain, is embedded with a variety of crypto 
tools such as: sMPC(secure multi-party computation), CP-ABE（ciphertext policy-Attribute 
Based Encrypyion), Full Homorphic Encryption and so on which supports the development
of many kinds of application with stringent privacy and security requirements.  Last but not least, 
Xinchain will integrate with AI technology to automatically manage Xinchain without human interference. 
In return, Xinchain provides computing power、data training(with privacy support) 
market for AI training.

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
Decentralized Public Ledger System: Xinchain. Any user can submit a transaction to this ledger 
system to bind a public key(can be viewed as identity key, generated from a ECDSA, RSA
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
and more people are begin to talk about Quantum Computers which use quantum mechanics to 
massively accelerate computation and how it may affect our lives. The research of quantum computers
opens the gates with new possibilities for many other research areas such as Machine learning, genetic 
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
combined with traditional ECDSA to offset the efficiency issue. There are few blockchain applications which bring in
quantum resistance such as HCash, QRL and efficiency is still the major concern.

In xinchain, we will investigate the newest post-quantum NIST proposal and find a most appropriate 
algorithm which is the best tradeoff between security, performance and key size. Then we will integrate
 it with our end to end encryption protocol: The key exchange part to guarantee quantum resistance. We 
 may need  traditional ECDSA in case efficiency is the issue. On the other hand, the 
encryption part we use AES which is already quantum-resistant don't need further change but updating
parameters to meet the security requirement. 


Off-chain storage system
========================
Currently, block-chain technologies has very limited applications since most of them(Bitcoin、Ethereum etc.) mainly 
served as a database of transactions which is very expensive for regular nodes to store(more than 200GB for bitcoin)
 and it will keep growing as time passes by.

On the other hand, the number of transactions be contained in a block is very limited since the maximum block size 
is fixed and each transaction takes up certain space. This situation leads to low tps(e.g. 7tps tops for bitcoin without 
segwit).

Consequently, one intuitive thought to make xinchain more scalable is not using it as storage medium but rather 
a tool to verify correctness. Complex logic should be kept outside of the xinchain. 

Xinchain platform and applications built on it will deal with these complex logic. User's 
application data（either encrypted or plaintext）will be stored on the distributed storage system like 
IPFS、 BitTorrent、Emule(With advantage over scalability and easy deployment, existing cloud storage like 
Dropbox, AWS can also be an option, but needs trust to a third-party). Unlike traditional internet service
 where users store their data on remote servers, users in Xinchain have absolute control over the access to
 their own data. Applications and other users can gain access to a user's data via a valida access token 
 issued and signed by this user if it's stored on cloud storage or a file hash if it's stored on distributed 
 file system. If the file is encrypted via AES, 3DES etc., extra decryption key will be required to access 
 the raw data. As to the policy of how to gain acess control over a user's data and under what circumstance
 the decryption key will be transfered, it's defined by a Xinchain smart contract. 

Confidential Transactions
============
Nowadays, users' privacy in the internet has been one of the most important issues. Some people may like sharing 
their lives with others: posting their daily activities, photos and moods on the internet(e.g. Facebook, twitter), 
but for the security sake, other users may like to keep their identity and activities in private：such as their 
names, address, shopping records etc. In the traditional internet service, we trust our privacy with third parties, 
upload our personal info to the remote servers through registration or basic browsing activity. But somehow, we 
still get constant harassment calls from telemarkters. The remote trusted servers is either compromised or our 
personal info is sold by the dishonest third parties. Anyway, they are pretty common currently.

A comibination of the decentrilization and crypto privacy technology can solve internet's privacy issue. For a start, 
we can see a transaction in bitcoin can pretty much hide sender and receiver's identity(sender and receiver are just 
a string of characters). However, bitcoin doesn't provide true anonymity. To achieve true confidentiality, we can see 
other efforts made in Dash, ZCash, Monero. Dash uses coinjoin to achieve anoymity but also require a trusted third party. 
ZCash uses zksnark which is pretty advanced zero knowledge proof technology but has following problems:
+ required a trusted setup(public CRS) which may leave a trapdoor;
+ process to generate a private transaction is very slow(more than 40s on a PC), therefore, private transaction is not 
enabled by default.

In Xinchain, we intend to use Ring-CT which is already used in Monero to ensure confidentiality. Ring-CT uses Ring signature 
developed by CryptoNote and Confidential transactions designed for Bitcoin by Greg Maxwell. It can protect sender and receiver's 
identities as well as transaction amount but has better performance than Zksnark. We will also make further optimization to this 
technology by improving efficiency and reducing transacion size. Some inovative ideas such as Zkstark will also be considered. 


Scalable Consensus Mechanism
============
Achieve consensus in a untrusted distributed system is quite a challenge. Traditional consensus mechanisms such as Paxos, Raft 
are run in a trusted environment which assume all nodes follow the protocol. However, in a untrusted environment, nodes can be 
malicious,  they may send corrupted messages to other nodes to mislead them.  

PBFT is a classic algorithm to solve this kind of problem， it can tolerate 1/3 corrupted Byzantine nodes but can only run in a 
small scale set of nodes. Proof of Work is a consensus algorithm Satoshi Nakamoto invented that can work on distributed system 
with a large number of nodes： Transactions in a period of time are packed in a block,  every node who wish to record this block 
needs to compute a hard math problem， whoever solve the problem gets the right to create a valid block and will get extra rewards. 
This algorithm's security relies on the computing power of the whole network. However,  this algorithm is slow to reach consensus、 
centralization caused by whale miner、 fork possibilities， 51% double spend etc. Most importantly, it's a huge waste of electric 
power. Other Consensus algorithms such as PoS, DPoS，dBFT are environment-friendly, but they are more centralized and have additional 
security issues.

It was widely believed there was a impossible triangle for Blockchain technology, namely: efficiency、security、decentralization. 
A blockchain can only meet at most two aspects. However, there was a breakthrough when Algorand team claimed their consensus algorithms 
could solve the impossible triangle. Algorand consensus algorithm's main innovation is committee selection based on VRF (A committee 
is a group of users who propose block and verify them). It sounds like DPoS, but more decentralized and secure. The committee selection 
are random and changes every round. Further more, the possibility of a node being selected is propotional to the coins it has which 
prevents sybil attack. Since Algorand, Difinity, Helix, ONT are also VRF based.

Xinchain's consensus algorithm will also be VRF-based but with further improvement, such as, without gossip broadcasting, the possibility 
of being selected are not propotional to the coins but also secure from sybil attack and so on.   

Other Scaling Solutions
============
Scalability has always been a problem for blockchain technology. For example， famous projects like bitcoin and ethereum， they are no 
match to the mainstream systems like visa and paypal：paypal manages 193 transactions per second and visa manages 1667 transactions per 
second. Ethereum does only 20 transactions per second while bitcoin only achieves 7 transactions per second before segwit!  

To improve scalability, we have seen lots of great technologies emerged, some of them have been put into applications such as SegWit, 
lightning network, DAG. Some of them are still in experiments, such as Raiden Network， Plasma.  

For Xinchain, we will apply some existing technologies which have universality. For example, the first thing comes to mind is 
Lightning network. It supports off-chain micropayments which can greatly enhance the scalability. Secondly, the Schnorr Signature scheme 
which is already implemented in Bitcoin Cash. It can reduce signature size of a transaction without sacrificing its security. 
Ergo, more transactions in a block which equals greater scalability.

Various Crypto Tools
============
As the Internet is being developed rapidly, the amount of data in our world is also increasing at an incredible speed. According to a recent 
report, it is estimated that 20% of the world’s data has been collected in the past couple of years. For example, Facebook, collected 300 
petabytes of personal data since its inception – a hundred times the amount the Library of Congress has collected in over 200 years. 

To sum up, we have entered the Big Data era, data is an extremely valuable asset as it can be analyzed to make some critical decisions, predict future,
advertising items users may find interesting, etc. Data leads to innovation and economic growth. On the other hand, privacy has also become a public 
concern as data grows. Centralized corporations amass large set of users' personal and sensitive data and user its self has very little control over this 
data and how it is used. Moreover, the increasingly rampant internet hacking activities have caused many personal info leakeage incidents. Hackers then sell
this info in the blacket to gain profit. 

Distributed technology like Xinchain offchain storage system can grant users control over their own personal data, but to further extend its application 
case with privacy needs. Users need more stronger cyrpto tools. Luckily, Xinchain Platform will support various crypto tools to meet all kind of privacy 
needs. For example, sMPC(secure multi-party computation) enable different nodes jointly compute a function over their inputs while keeping those inputs 
private. This is a pretty useful gadget in the data trading market: you want to rent your data to someone else, allow them do some computation on the 
data without revealing the raw data.  

Other useful crypto tools such as k-anonymity、differential privacy，full homorphic encryption， partially homorphic encryption, zero knowledge proof, 
ring signature, attribute-based encryption, blind signature, Intel SGX etc. will also be supported to suffice various application requirements.

These crypto tools will be plugabble, user can configure what they want to install to meet their specific needs. 


Xinchain and AI
============
Artificial Intelligence and Blockchain are both revolutionized technologies and have wide applicability. They can both change the way how traditional 
trades work. In Xinchain, we want to combine these two technologies to server each other realizing their full potential.  

#AI serves xinchain
For a classic blockchain to work, certain rules need to be made before launch the whole network. These rules are called consensus. However, even though 
some rules are out-dated, once they are settled, it's nearly impossible for them to be changed. It requires a hard-fork upgrade to change a certain rule. 
Miners、exchanges、regular nodes need to reach an agreement on this issue, otherwise, the chain will inevitably split into two chains(e.g. BCH). Some projects 
like EOS can dynamically change consensus rules by voting but still lots of human work are involed with limited choices.

However, with AI, the network as a whole can adjust more intelligently and dynamically to fullfill users' needs. For example, dynamically changing block size, 
block interval and PoW difficulty to resolve scaling problem and avoid dishonest miners jumping on and off.  

Also, AI can be used monitoring misbehave nodes(keep sending invalid transactions or blocks) and banning them(traditionally, misbehave nodes are banned manully 
or hard-coded into the node). Plus, AI can be used to detect unexpected situation and warn users, such as network partition, DDoS attack, fraudlent activity 
etc. 


#Xinchain serves AI
Xinchain can also serve the development of AI.

##Leverage Computing resouces over the blockchain network, Blockchain Help Us Build a Distributed Supercomputer
AI tech is a fascinating tech. It can open up a lot of doors for us. However, it requires a lot of computation power and too expensive 
for small companies and individuals to pursue. 

Firstly, you need to purchase super-fast and powerful CPU, GPU, SSD, and powerful motherboard and keep buying them once more powerful new
model comes to market. Needless to say, these aren’t cheap. Moreover, renting server room, enormous power supply to keep it running and 
air-conditioning, maintenance bills will come along with it.

Anyhow, it's a common knowledge that AI’s require a lot of computing powers and computing powers are becoming a scarcity now. On the other hand, 
blockchain amasses a lot of computating power and can deploy them globally. E.g., miners across the world mine bitcoin together. Since, PoW is 
going to be obsolete, a lot of computing power will be idle. However, with Xinchain distributed ledger, computing power can redeploy to do AI 
computation. Via Xinchain smart contract, without a trusted party, users lend their computing power and get certain tokens as rewards. Individuals 
or companies who rent these computing power around the network to do AI training together needs to pay but at a much smaller price than buying 
hardware themselves. 

Also Xinchain's offchain storage system supports  data marketplace for AI trainning. However, do AI training in a open decentralized network, privacy 
on the training data are required for the general cases. 

Xinchain's various crypto tools can meet the training data privacy requirement.  
+ By Federated Learning(Google) and Differential Privacy(Apple), users can do AI trainning without actually upload the authentic raw trainning data.
+ By multi-party computation and Hormophic encryption, nodes can do AI trainning on encrypted training data.  

