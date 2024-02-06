from src.data.Frame.IFrameRepositoryKeyValue import IFrameRepositoryKeyValue
from src.data.Frame.IFrameRepositoryDocument import IFrameRepositoryDocument
from .RepositoryAbstractFactory import RepositoryAbstractFactory
from src.data.Frame.FrameRepositoryRedis.FrameRepositoryRedisKeyValue import FrameRepositoryRedisKeyValue
from src.data.Frame.FrameRepositoryRedis.FrameRepositoryRedisDocument import FrameRepositoryRedisDocument

class RepositoryFactoryRedis(RepositoryAbstractFactory):
  def createFrameRepositoryKeyValue(self) -> IFrameRepositoryKeyValue:
    return FrameRepositoryRedisKeyValue()
  
  def createFrameRepositoryDocument(self) -> IFrameRepositoryDocument:
    return FrameRepositoryRedisDocument()