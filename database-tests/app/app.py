from src.RepositoryFactory.RepositoryFactoryMapping import repository_factory_mapping
from src.RepositoryFactory.RepositoryAbstractFactory import RepositoryAbstractFactory
from src.business.Frame.Frame import Frame 

from src.data.Frame import seed
import time

def print_database_performance_after_test(frame_repository, number_of_frames : int, total_time):
    print(f'Time elapsed: {total_time * 1000} milliseconds')
    print(f'Number of frames saved: {number_of_frames}')
    print(f'Average time per frame: {total_time * 1000 / number_of_frames} milliseconds')

    data_size = frame_repository.get_data_size()
    print("Total Data Size: {dataSize} bytes".format(dataSize=data_size))
    print("Avg. Obj Size: {avgObjSize} bytes".format(avgObjSize=data_size/number_of_frames))

    print("Object sample (id: 0):")
    print(frame_repository.find("0"))

def test_write_performance_key_value(repositoryFactory : RepositoryAbstractFactory, default_frames, number_of_frames : int):
    frame_repository = repositoryFactory.createFrameRepositoryKeyValue()
    print("Starting write performance test using {repositoryName}.".format(repositoryName=frame_repository.repository_name()))

    # delete all frames before starting the test
    frame_repository.delete_all()

    start = time.time()
    for frame in default_frames:
        try :
            frame_repository.save(frame.id, str(frame))
        except Exception as e:
            print("Error while saving frame: {error}".format(error=e))
            print("Frame: {frame}".format(frame=frame))
            break
    end = time.time()
    
    print("Saved!")
    total_time = end - start
    
    print_database_performance_after_test(frame_repository, number_of_frames, total_time)
    

def main():
    print("Client main ENTRY")
    REPOSITORY_TYPE = "mongodb"

    repositoryFactory = repository_factory_mapping[REPOSITORY_TYPE]
    default_frames = seed.frames_with_different_robots
    number_of_frames = seed.NUMBER_OF_FRAMES

    test_write_performance_key_value(repositoryFactory, default_frames, number_of_frames)
    print("Client main EXIT")

if __name__ == "__main__":
    main()