
//
// traffic - A Simple Network Simulator
// Copyright (c) Dave Kilian 2014
//
// traffic.h - Public API header
//

#ifndef TRAFFIC_H
#define TRAFFIC_H

#include <stdbool.h>


// 
// Forward declarations for types
//

typedef int   tr_err;      // An error code
typedef void *tr_conf;     // A traffic config file
typedef void *tr_network;  // A network topology
typedef void *tr_node;     // A simulated machine
typedef void *tr_iface;    // A network interface on a simulated machine
typedef void *tr_link;     // A link between simulated network interfaces


// 
// Error codes
//

static const tr_err TR_OK = 0;
static const tr_err TR_ENOTFOUND = -1;
static const tr_err TR_EARRAYLEN = -2;
static const tr_err TR_ENAMETAKEN = -3;
static const tr_err TR_ENETINUSE = -4;
static const tr_err TR_EPOINTER = -5;
static const tr_err TR_ESTACKEMPTY = -6;
static const tr_err TR_EOUTOFRANGE = -7;

// Gets an English string explaining the given error code
//
const char *tr_errstr(tr_err error);


//
// Configuration files
//

// Opens the configuration file at the given path.
// If successful, returns TR_OK and sets conf.
// Otherwise returns an error and does not modify conf.
//
tr_err tr_conf_open(const char *path, tr_conf *conf);

// Parses the network topology present in the given config file.
// If successful, returns TR_OK and sets net.
// Otherwise returns an error and does not modify net.
//
tr_err tr_conf_load(tr_conf conf, tr_network *net);

// Serializes the given network to the config file
//
tr_err tr_conf_write(tr_conf conf, tr_network net);

// Closes the configuration file, releasing any locks on the underlying file.
// Frees any memory in use by the conf object and invalidates it,
// but does not invalidate any networks loaded from the conf object.
//
void tr_conf_close(tr_conf conf);


//
// Network modeling
//

// When assigned to a network interface, specifies that traffic should choose a
// MAC address for the adapter when the network simulation is bound.
//
static const char * const TR_ANY_MAC_ADDR = 0;

// When assigned to a network interface, specifies that traffic should choose
// an IP address for the adapter when the network simulation is bound.
//
static const char * const TR_ANY_IP_ADDR = 0;

// When assigned to a network interface, specifies that traffic should choose
// an IP subnet for the adapter when the network simulation is bound.
//
static const int TR_ANY_SUBNET_MASK = -1;


// Allocates and returns a network toplogy with nothing in it.
// The name parameter specifies a friendly name of the network.
// If you specify NULL for name, the network will not be named.
//
tr_network tr_net_create(const char *name);

// Gets the friendly name of this network.
// Returns NULL if the network isn't named.
//
const char *tr_net_name();

// Gets the number of nodes in this network
//
int tr_net_num_nodes(tr_network net);

// Fills the given array with the nodes in this network.
// len indicates the length of the given array.
// If the array is too small, this fails with TR_E
//
tr_err tr_net_nodes(tr_network net, tr_node *nodes, int len);

// Adds a link between network interfaces i1 and i2.
// link receives an object representing the physical link between the two.
//
tr_err tr_net_link(tr_network net, tr_iface i1, tr_iface i2, tr_link *link);

// Indicates whether there is a node with the given unqiue ID on this network
//
bool tr_net_has_node(const char *name);

// Gets the node with the given name.
// If there is node with the given name, returns NULL.
//
tr_node tr_net_node(const char *name);

// Frees memory in use by the network topology, its nodes, their interfaces,
// and the links between those interfaces.
//
tr_err tr_net_delete(tr_network net);


// Adds a new node to the given network.
// If name is NULL, traffic will assign the node a unique name.
// Otherwise, name must unique among all other names in the network.
//
tr_node tr_node_create(tr_network net, const char *name);

// Gets the unique ID of the given node.
// This value is never NULL.
//
const char *tr_node_name(tr_node node);

// Gets the network this node belongs to.
// This value is never NULL.
//
tr_network tr_node_network(tr_node node);

// Gets the number of virtual network interfaces attached to this virtual node.
//
int tr_node_num_ifaces(tr_node node);

// Gets the virtual network interfaces attached to this virtual node.
// len is the length of the array to fill.
// If the array is not long enough, this returns TR_EARRAYLEN.
//
tr_err tr_node_ifaces(tr_node node, tr_iface *ifaces, int len);

// Indicates whether the node has an interface with the given unique ID
//
bool tr_node_has_iface(tr_node node, const char *name);

// Gets the node's interface with the given name.
// If there is no such node, returns NULL.
//
tr_iface tr_node_iface(tr_node node, const char *name);

// TODO work out how we'll model routing behavior and running applications

// Deletes this node and removes it from the network it belongs to.
//
tr_err tr_node_delete(tr_node node);


// Adds a virtual network interface to the given virtual node.
// If name is NULL, traffic will assign the interface a unique name.
// Otherwise, name must unique among all other names in the network.
//
tr_iface tr_iface_create(tr_node node, const char *name);

// Gets the node the network interface belongs to
//
tr_node tr_iface_node(tr_iface iface);

// Gets the unique ID of the network interface
//
const char *tr_iface_name(tr_iface iface);

// Gets the MAC address that will be assigned to this network interface 
// when the virtual network is bound. If the value is TR_ANY_MAC_ADDR, 
// traffic will choose a unique MAC for the interface when it is bound.
//
// The default value for this parameter is TR_ANY_MAC_ADDR.
// Otherwise the value will be a list of hex bytes ("01:23:45:67:89:ab").
//
const char *tr_iface_mac(tr_iface iface);

// Sets the MAC address that will be assigned to this network interface 
// when the virtual network is bound. If the value is TR_ANY_MAC_ADDR, 
// traffic will choose a unique MAC for the interface when it is bound.
//
// The default value for this parameter is TR_ANY_MAC_ADDR.
// Otherwise the value must be a list of hex bytes ("01:23:45:67:89:ab").
//
tr_err tr_iface_set_mac(tr_iface iface, const char *macaddr);

// Gets the IP address that will be assigned to this network interface 
// when the virtual network is bound. If the value is TR_ANY_IP_ADDR, 
// traffic will choose a unique IP for the interface when it is bound.
//
// The default value for this parameter is TR_ANY_IP_ADDR.
// Otherwise the value will be a list of decimal bytes ("111.222.33.4").
//
const char *tr_iface_ip(tr_iface iface);

// Sets the IP address that will be assigned to this network interface 
// when the virtual network is bound. If the value is TR_ANY_IP, 
// traffic will choose a unique IP for the interface when it is bound.
//
// The default value for this parameter is TR_ANY_IP_ADDR.
// Otherwise the value must be a list of decimal bytes ("111.222.33.4").
//
tr_err tr_iface_set_ip(tr_iface iface, const char *ip);

// Gets the subnet mask that will be assigned to this network interface
// when the virtual network is bound. If the value is TR_ANY_SUBNET,
// traffic will choose an appropriate subnet mask for the interface.
//
// The default value for this parameter is TR_ANY_SUBNET_MASK.
// Otherwise the value must be the number of IP address bits
// that are common to all nodes in the subnet.
//
int tr_iface_subnet_mask(tr_iface iface);

// Sets the subnet mask that will be assigned to this network interface
// when the virtual network is bound. If the value is TR_ANY_SUBNET,
// traffic will choose an appropriate subnet mask for the interface.
//
// The default value for this parameter is TR_ANY_SUBNET_MASK.
// Otherwise the value must be the number of IP address bits
// that are common to all nodes in the subnet.
//
tr_err tr_iface_set_subnet_mask(tr_iface iface, int subnet);

// Gets the number of other network interfaces this interface is connected to
//
int tr_iface_num_links(tr_iface iface);

// Gets the other network interfaces this interface is connected to.
// len is the length of the links array.
// If the link array isn't long enough, fails with TR_EARRAYLEN.
//
tr_err tr_iface_links(tr_iface iface, tr_link *links, int len);

// Indicates whether the interfaces are linked to each other
//
bool tr_iface_has_link(tr_iface iface, tr_iface other);

// Gets the link object describing the link between the two interfaces.
// If there is no link between the two, returns NULL.
//
tr_link tr_iface_link(tr_iface iface, tr_iface other);

// Deletes this interface from its node
//
tr_err tr_iface_delete(tr_iface iface);


// Gets one of the interfaces connected by this link
// Index must be 0 or 1; otherwise this returns NULL.
//
tr_iface tr_link_endpoint(tr_link link, int index);

// Gets the artificial mean latency to apply to this link, in milliseconds.
// The default value is 0 (instant delivery).
//
long tr_link_latency(tr_link link);

// Sets the artificial mean latency to apply to this link, in milliseconds.
// The default value is 0 (instant delivery)
//
tr_err tr_link_set_latency(tr_link link, long latency);

// Gets the artifical latency variance to aply to this link, in milliseconds
// The default value is 0 (delivery time = mean latency).
//
long tr_link_variance(tr_link link);

// Sets the artifical latency variance to aply to this link, in milliseconds
// The default value is 0 (delivery time = mean latency).
//
tr_err tr_link_set_variance(tr_link link, long variance);

// Gets the ratio of packets that are dropped along this link.
// The default value is 0 (deliver all packets)
//
float tr_link_droprate(tr_link link);

// Sets the ratio of packets that are dropped along this link.
// The default value is 0 (deliver all packets)
//
tr_err tr_link_set_droprate(tr_link link, float droprate);

// Indicates whether this link is enabled.
// A disabled link does not ferry any traffic (as if a cable was unplugged).
//
bool tr_link_is_enabled(tr_link link);

// Enables a link.
// A disabled link does not ferry any traffic (as if a cable was unplugged).
//
tr_err tr_link_enable(tr_link);

// Disables a link.
// A disabled link does not ferry any traffic (as if a cable was unplugged).
//
tr_err tr_link_disable(tr_link);

// Deletes this link, disconnected the interfaces the link once connected.
//
tr_err tr_link_delete(tr_link link);


//
// Network simulations
//

// Creates virtual network devices on the host machine for this network.
// A network device will be created for each tr_iface in the tr_network.
// This does not actually begin the network simulation,
// so no packets will be moving through the bound network.
//
tr_err tr_net_bind(tr_network net);

// Indicates whether virtual network devices for this network have been created
// on the host operating system.
//
bool tr_net_is_bound(tr_network net);

// Begins network simulation.
// If the nework isn't bound, calls tr_net_bind for you.
// Then launches any node apps and begins routing packets.
//
tr_err tr_net_start(tr_network net);

// Indicates whether the virtual network is bound and routing packets
//
bool tr_net_is_simulating(tr_network net);

// Ends the network simulation, but leaves the network bound
//
tr_err tr_net_stop(tr_network net);

// Deletes all virtual network devices that were created for this network.
// The network simulation must not be running; otherwise returns TR_ENETINUSE.
//
tr_err tr_net_unbind(tr_network net);

// Indicates whether the node is part of a bound network
//
bool tr_node_is_bound(tr_node node);

// Indicates whether the interface is part of a bound network
bool tr_iface_is_bound(tr_iface iface);

// Gets the MAC address of the host OS virtual network device for the iface.
// If the iface isn't bound, returns NULL.
// Otherwise returns a textual MAC (01:23:45:67:89:ab)
//
const char *tr_iface_cur_mac(tr_iface iface);

// Gets the IP address of the host OS virtual network device for the iface.
// If the iface isn't bound, returns NULL.
// Otherwise returns a textual IPv4 address (123.456.78.90)
//
const char *tr_iface_cur_ip(tr_iface iface);

// Gets the subnet mask of the host OS virtual network device for the iface.
// If the iface isn't bound, returns -1.
// Otherwise returns the number of prefix IP bits shared by all nodes in the
// subnetwork.
//
int tr_iface_cur_subnet_mask(tr_iface iface);


//
// Simulation monitoring
//

// TODO - polling model for aggregate statistics (must be explicitly enbaled)
// TODO - subscriber model for events (granular - e.g. packet drops/delivery)

#endif
