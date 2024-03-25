FROM mongo:7

# Create a shell script to execute multiple commands
RUN echo '#!/bin/bash' > setup.sh && \
    echo 'mongod --replSet rs0 --bind_ip localhost,mongodb1 &' >> setup.sh && \
    echo 'sleep 10' >> setup.sh && \
    echo 'mongosh --eval "rs.initiate({_id: \"rs0\",members: [{_id: 0, host: \"mongodb1\"},{_id: 1, host: \"mongodb2\"},{_id: 2, host: \"mongodb3\"}]})"' >> setup.sh && \
    chmod +x setup.sh

# Run the shell script when the container starts
CMD ["./setup.sh"]