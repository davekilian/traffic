
# traffic

traffic is a simple netowrk simulator. On a single machine, traffic creates a
virtual network of nodes running your application code. This can be handy for
implementing distributed systems, since you don't need to get a bunch of
machines together or deal with link-layer quirks.

## Installing

There currently aren't any binary distributions of traffic, so the only way to
set it up on your machine is to build from source. This requires a POSIX
toolchain. To install traffic, simply

    $ git clone https://github.com/davekilian/traffic.git
    $ cd traffic
    $ make
    $ sudo make install

## Using

traffic works by defining a virtual network topology using 
[TAP adapters](http://en.wikipedia.org/wiki/TUN/TAP), and managing applications
that bind to these adapters. You give traffic a configuration file that tells 
it how to set up the network and start your code.

In the future, you will be able to use a graphical utility to build these
configuration files. In the meantime, however, you will need to write them by
hand.

### TODO

For now this level of detail is fine, but long-term we want to migrate some of
the details to a `docs/` subdir so readers here can get just the big picture.

### Defining Network Topologies

Network topologies are defined textually in two steps. First, you define a set
of nodes in your topology. Second, you define the links between these nodes.
Consider the following sample topology:

    node 'A' { 
        interface 'AB' { mac 'auto' ip 'auto' subnet 'auto' }
    }

    node 'B' {
        interface 'BA'
        interface 'BC'
    }

    node 'C' {
        interface 'CB'
    }

    link { from 'AB' to 'BA' }
    link { from 'BC' to 'CB' latency '150 ms' variance '75 ms' }

Each node represents a single machine, with one or more network interfaces.
These interfaces are connected through links. You can give nodes, interfaces
and links unique IDs. If you omit an ID, traffic will choose one at runtime.

The `mac`, `ip` and `subnet` parameters specify the interface's MAC address,
static IP, and subnet, respectively. Specifying `'auto'` or omitting the param
lets traffic choose a default.

Currently `latency` and `variance` are the only way to tweak the physical
characteristics of a data link. In the future, these parameters may be
expanded, and/or a plugin model will give you fine-grained control of physical
link behavior.

### Node Behavior

The example above defines a network topology, but no packets can move through
the network because nobody creates any!

Once traffic has finished bringing up the virtual network, it executes commands
stored in your configuration file. Each node can have its behavior defined by
just one of the commands below: 

#### `hub`

Hubs replay any packet they receive on one interface to all other interfaces.
Hubs don't have any special parameters, so you can just add a `hub` declaration
to your `node`:

    node `myhub` {
        interface 'hub1'
        interface 'hub2'
        interface 'hub3'
        hub
    }

#### `switch`

Switches route packets received on one interface to the interface via which the
recipient can be reached. To create a switch with default parameters, just add
a `switch` declaration:

    node `aswitch` {
        interface 's1'
        interface 's2'
        interface 's3'
        switch
    }

You can add optional arguments to the switch:

* `strategy` controls how the switch learns routes. You can specify
  `'storeAndForward'`, `'cutThrough'`, or `fragmentFree` to use a well-known
  switching algorithm, or `'psychic'` to create the perfect switch that routes
  using traffic's internal topology data. The default option is `'psychic'`.

* `noroute` controls what psychic switches do when there is no route to the
  desired host. Valid options are `'drop'` or `'broadcast'`. The default is
  `'broadcast'`

For example:

    node `switch2' {
        interface 'alpha'
        interface 'beta'
        interface 'gamma'
        switch { strategy 'psychic' noroute 'drop' }
    }

#### `router` and `gateway`

`router`s bridge subnets and optionally provide network address translation.
`gateway`s bridge your virtual network with your machine's physical network.
The syntax for these node types have been omitted for brevity. See the `docs/`
folder in the repo instead.

#### `app`

`app` specifies the command(s) that you want to 'run' on a node. In reality,
all commands will run on your local machine simultaneously. You specify macros
in the command line to allow traffic to tell the application which local MAC
address or IP address (if applicable) to bind to:

    node 'A' {
        interface 'AB'
        app { 
            command '/usr/bin/my-echo --bindto=$('AB'.ip)'
        }
    }

### Bringing Up the Network

traffic is a shared library that publishes an API (see `inc/traffic.h`). It 
also comes with a binary (called 'traffic') that uses that API to bring 
networks up and down. To bring up a network, give traffic the path to your 
network configuration:

    $ traffic mynet.conf

`traffic` will stay open while your virtual network operates. To bring down the
virtual network, send `traffic` a SIGINT (e.g. via Ctrl+C). 

While the network is running, `traffic` will open a channel to allow monitoring
programs to inspect the state of the network (e.g. to check health, collect
statistics and run visualizations). To learn more, see `docs/monitoring.md`).

## Developing

Since this is a very young project, this section is pretty bare :)

### Environment

traffic is written in C. The control over memory layout and allocation that C 
gives us helps us maximize virtual network throughput.
For simplicity, we don't use a complex third-party build system to compile the
application. Users are able to install traffic with basic build tools and a few
lines of shell.

### Architecture
### Bugs and Feature Requests
### Contributing Patches
