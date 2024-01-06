from src.RepositoryFactory.RepositoryFactoryMapping import repository_factory_mapping
from src.data.Frame import seed
import time

def main():
    REPOSITORY_TYPE = "mongodb"
    repository = repository_factory_mapping[REPOSITORY_TYPE]
    frame_repository = repository.createFrameRepository()
    default_frames = seed.frames_with_different_robots
    number_of_frames = seed.NUMBER_OF_FRAMES

    print("Using {REPOSITORY_TYPE} repository...".format(REPOSITORY_TYPE=REPOSITORY_TYPE))

    print("Saving frames...")
    start = time.time()
    for frame in default_frames:
        frame_repository.save(frame)
    end = time.time()
    print("Saved!")

    total_time = end - start
    print(f'Time elapsed: {total_time * 1000} milliseconds')
    print(f'Number of frames saved: {number_of_frames}')
    print(f'Average time per frame: {total_time * 1000 / number_of_frames} milliseconds')

if __name__ == "__main__":
    main()