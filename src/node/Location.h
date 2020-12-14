#ifndef SRC_NODE_LOCATION_H_
#define SRC_NODE_LOCATION_H_

namespace node {

class Location {
public:
	int line;

	int column;

	Location(int line, int column);
	virtual ~Location();
};

} /* namespace node */

#endif /* SRC_NODE_LOCATION_H_ */
