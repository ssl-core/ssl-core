from ....business.Frame.Frame import Frame
from cassandra.cluster import Cluster
from cassandra.query import SimpleStatement, ConsistencyLevel
from cassandra.auth import PlainTextAuthProvider
import json
from src.data.Frame.IFrameRepositoryDocument import IFrameRepositoryDocument
from src.business.Frame.Frame import Frame


class FrameRepositoryCassandraDocument(IFrameRepositoryDocument):
    def __init__(self):
        super().__init__()
        try:
            self._name = "cassandra"

            auth_provider = PlainTextAuthProvider(username='your_username', password='your_password')  # Replace with your Cassandra credentials
            self.cluster = Cluster(['cassandra'], auth_provider=auth_provider)  # Use the service name as the host
            self.session = self.cluster.connect()
            
            print("Drop the table if it exists. (Cassandra)")
            self.session.execute("DROP TABLE IF EXISTS frames_table")

            # Replace 'your_keyspace' with the actual keyspace name
            self.session.execute("CREATE KEYSPACE IF NOT EXISTS your_keyspace WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 1}")
            self.session.set_keyspace('your_keyspace')
            
            # Replace 'frames_table' with your actual table name
            self.session.execute("CREATE TABLE IF NOT EXISTS frames_table (frame_id text PRIMARY KEY, frame_data text)")
        except Exception as e:
            print(f"Could not connect to Cassandra: {e}")

    def repository_name(self) -> str:
        return self._name

    def save(self, frame: Frame) -> None:
        # convert Frame to json and save it to cassandra
        frame_id = frame.id
        try:
            frame_data = json.dumps(frame.__dict__)
        except TypeError:
            print("Frame object contains non-serializable attributes.")
        else:
            query = "INSERT INTO frames_table (frame_id, frame_data) VALUES (%s, %s)"
            statement = SimpleStatement(query, consistency_level=ConsistencyLevel.ONE)
            self.session.execute(statement, (str(frame_id), str(frame_data)))

    def find(self, frame_id: str) -> Frame:
        query = "SELECT * FROM frames_table WHERE frame_id = ?"
        statement = SimpleStatement(query, consistency_level=ConsistencyLevel.ONE)
        result = self.session.execute(statement, (frame_id,))
        row = result.one()
        if row:
            return Frame(**json.loads(row['frame_data']))
        else:
            print("Frame not found.")

    def update(self, frame: Frame) -> Frame:
        pass

    def delete(self, frame: Frame) -> None:
        pass

    def delete_all(self) -> None:
        print("Drop the table if it exists. (Cassandra)")
        self.session.execute("DROP TABLE IF EXISTS frames_table")

    def get_data_size(self):
        # get the data size used by the table
        query = "SELECT * FROM system_schema.tables WHERE keyspace_name = 'your_keyspace' AND table_name = 'frames_table'"
        statement = SimpleStatement(query, consistency_level=ConsistencyLevel.ONE)
        result = self.session.execute(statement)
        row = result.one()
        if row:
            return row['total_size']
        else:
            print("Table not found.")