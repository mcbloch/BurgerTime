from databases import Database
import sqlalchemy
from pydantic import BaseModel

DATABASE_URL = "sqlite:///./test.sqlite"

database = Database(DATABASE_URL)

metadata = sqlalchemy.MetaData()

highscores = sqlalchemy.Table(
    "highscores",
    metadata,
    sqlalchemy.Column("id", sqlalchemy.Integer, primary_key=True),
    sqlalchemy.Column("name", sqlalchemy.String),
    sqlalchemy.Column("score", sqlalchemy.Integer),
)

engine = sqlalchemy.create_engine(
    DATABASE_URL, connect_args={"check_same_thread": False}
)
metadata.create_all(engine)

class HighscoreIn(BaseModel):
    name: str
    score: int

class Highscore(BaseModel):
    id: int
    name: str
    score: int