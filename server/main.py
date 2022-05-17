from typing import Union, List

from fastapi import FastAPI

from db import Highscore, HighscoreIn, database, highscores

app = FastAPI()

@app.on_event("startup")
async def startup():
    await database.connect()

@app.on_event("shutdown")
async def shutdown():
    await database.disconnect()


@app.get("/highscores/", response_model=List[Highscore])
async def read_highscores(limit: int = 10, sort_by: str = "score", sort_descending: bool = True):
    order_col = highscores.c[sort_by]
    order = order_col.desc() if sort_descending else order_col.asc()
    query = highscores.select().limit(limit).order_by(order)
    return await database.fetch_all(query)


@app.post("/highscores/", response_model=Highscore)
async def create_highscore(highscore: HighscoreIn):
    query = highscores.insert().values(name=highscore.name, score=highscore.score)
    last_record_id = await database.execute(query)
    return {**highscore.dict(), "id": last_record_id}