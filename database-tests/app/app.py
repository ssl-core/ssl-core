from src.RepositoryFactory.RepositoryFactoryMapping import repository_factory_mapping
from src.data.Frame import seed
import time

def test_write_performance(frame_repository, default_frames, number_of_frames, save_serialized=False):
    # clears the database before starting the test
    print("Starting write performance test using {repositoryName} (save_serialized = {saveSerialized}).".format(repositoryName=frame_repository.repository_name(), saveSerialized=save_serialized))
    
    frame_repository.delete_all()
    
    save_method = frame_repository.save_serialized if save_serialized else frame_repository.save
    
    start = time.time()
    for frame in default_frames:
        save_method(frame)
    end = time.time()
    
    print("Saved!")

    total_time = end - start
    print(f'Time elapsed: {total_time * 1000} milliseconds')
    print(f'Number of frames saved: {number_of_frames}')
    print(f'Average time per frame: {total_time * 1000 / number_of_frames} milliseconds')

    data_size = frame_repository.get_data_size()
    print("Total Data Size: {dataSize} bytes".format(dataSize=data_size))
    print("Avg. Obj Size: {avgObjSize} bytes".format(avgObjSize=data_size/number_of_frames))

    print("Object sample (id: 0):")
    print(frame_repository.find("0"))

def main():
    print("Client main ENTRY")
    REPOSITORY_TYPE = "mongodb"

    repository = repository_factory_mapping[REPOSITORY_TYPE]
    frame_repository = repository.createFrameRepository()
    default_frames = seed.frames_with_different_robots
    number_of_frames = seed.NUMBER_OF_FRAMES

    test_write_performance(frame_repository, default_frames, number_of_frames)
    test_write_performance(frame_repository, default_frames, number_of_frames, save_serialized=True)
    print("Client main EXIT")

if __name__ == "__main__":
    main()