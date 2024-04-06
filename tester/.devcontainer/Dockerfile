FROM robocin/ssl-core-python-base:latest

RUN pip3 install protobuf

COPY protocols/ /tmp/protocols

RUN cd /tmp/protocols && rm -rf /opt/protocols && mkdir /opt/protocols && \
  for file in $(find . -type f -name *.proto); do protoc --python_out=/opt/protocols -I. $file; done && \
  rm -rf /tmp/protocols

ENV PYTHONPATH=$PYTHONPATH:/workspaces/tester:/opt/protocols

COPY requirements.txt requirements.txt
RUN pip3 install -r requirements.txt
