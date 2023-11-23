from scylla.cluster import Session
from scylla.query import SimpleStatement

from src.data.Frame.IFrameRepository import IFrameRepository
from src.business.Frame.Frame import Frame

class FrameRepositoryScylla(IFrameRepository):
    def __init__(self):
        super().__init__()
        try:
            self.cluster = Session(['scylla'], keyspace='your_keyspace', port=9043)  # Use the service name as the host
            self.session = self.cluster.connect()
            
            # Replace 'your_keyspace' with the actual keyspace name
            self.session.execute("CREATE KEYSPACE IF NOT EXISTS your_keyspace WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 3}")
            self.session.set_keyspace('your_keyspace')
            
            # Replace 'frames_table' with your actual table name
            self.session.execute("CREATE TABLE IF NOT EXISTS frames_table (frame_id text PRIMARY KEY, frame_data text)")
        except Exception as e:
            print(f"Could not connect to ScyllaDB: {e}")

    def save(self, frame: Frame) -> None:
        frame_id = frame.id
        try:
            frame_data = str(frame)
        except TypeError:
            print("Frame object contains non-serializable attributes.")
        else:
            query = "INSERT INTO frames_table (frame_id, frame_data) VALUES (?, ?)"
            statement = SimpleStatement(query)
            self.session.execute(statement, (frame_id, frame_data))
