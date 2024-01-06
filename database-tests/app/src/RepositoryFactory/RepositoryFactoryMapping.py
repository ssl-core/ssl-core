from .RepositoryFactoryRedis import RepositoryFactoryRedis
from .RepositoryFactoryMongoDB import RepositoryFactoryMongoDB
from .RepositoryFactoryCassandra import RepositoryFactoryCassandra

repository_factory_mapping = {
    'redis': RepositoryFactoryRedis(),
    'mongodb' : RepositoryFactoryMongoDB(),
    'cassandra' : RepositoryFactoryCassandra()
}