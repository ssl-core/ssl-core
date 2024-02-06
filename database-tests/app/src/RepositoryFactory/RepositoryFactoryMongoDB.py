from src.data.Frame.IFrameRepositoryKeyValue import IFrameRepositoryKeyValue
from src.data.Frame.IFrameRepositoryDocument import IFrameRepositoryDocument
from .RepositoryAbstractFactory import RepositoryAbstractFactory
from src.data.Frame.FrameRepositoryMongoDB.FrameRepositoryMongoDBKeyValue import FrameRepositoryMongoDBKeyValue
from src.data.Frame.FrameRepositoryMongoDB.FrameRepositoryMongoDBDocument import FrameRepositoryMongoDBDocument

class RepositoryFactoryMongoDB(RepositoryAbstractFactory):
  def createFrameRepositoryKeyValue(self) -> IFrameRepositoryKeyValue:
    return FrameRepositoryMongoDBKeyValue()
  
  def createFrameRepositoryDocument(self) -> IFrameRepositoryDocument:
    return FrameRepositoryMongoDBDocument()