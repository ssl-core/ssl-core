#!/bin/bash

# Start MongoDB with replica set configuration
mongod --replSet rs0 --bind_ip localhost,mongodb1 &

# Wait for MongoDB to start
sleep 10

# Initialize the replica set
mongosh --eval "rs.initiate({_id: \"rs0\",members: [{_id: 0, host: \"mongodb1\"},{_id: 1, host: \"mongodb2\"},{_id: 2, host: \"mongodb3\"}]})"
