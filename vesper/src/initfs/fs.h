/**
* Associative filesystem interface.
* Defines a namespaced set of file objects with arbitrary metadata attributes.
* There are no directories, directories can be emulated by any UI layer.
* Views or file groups are used instead on the fs level. Views define a filter
* that is used to pick out arbitrary file objects from the whole filesystem
* given a filtering expression.
**/

/**
* Whole filesystem interface.
**/
struct com_ifilesystem
{
};

/**
* Filesystem node interface.
* Defines file-related operations on a node.
**/
struct com_ifsnode
{
	open /* return a fsobject (stream, char or block) interface for manipulating
	data contained in this node. */
	metadata /* return a record interface for manipulating metadata records */
	// security interface?
};

/**
* Filesystem view (filter that selects only files corresponding to a given criterion).
**/
struct com_ifsview
{
	filter /* manipulate filtering expression */
	iterator /* enumerate all fsnodes matching current expression (present in view) */
};