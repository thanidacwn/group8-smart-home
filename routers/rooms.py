from fastapi import APIRouter, Body, HTTPException
from fastapi.encoders import jsonable_encoder
from pydantic import BaseModel
from database.mongo_connection import *
from typing import Dict

router = APIRouter(
    prefix="",
    tags=["rooms"],
    responses={404: {"description": "Not found"}},
)

class Room(BaseModel):
    """ Room model """
    state: int
    brightness: int
    is_auto: int

class Data(BaseModel):
    room1: Room
    room2: Room
    room3: Room


def binary_to_percentage(binary):
    """ Convert binary to percentage """
    return round(binary / 2.55)

def percentage_to_binary(percentage):
    """ Convert percentage to binary """
    return round(percentage * 2.55)


def check_exception(data: Room):
    if data.state not in [0, 1]:
        raise HTTPException(status_code=400, detail="Incorrect state.")
    if data.brightness not in range(0,101):
        raise HTTPException(status_code=400, detail="Incorrect brightness.")
    if data.is_auto not in [0, 1]:
        raise HTTPException(status_code=400, detail="Incorrect status for light control")


@router.get("/rooms")
def get_all_rooms():
    """ Get all rooms """
    rooms = collection.find({"room_num":  True})
    return rooms

@router.post("/update_rooms")
def update_all_rooms():
    """ Update all rooms """
    rooms = collection.find({"room_num":  True})
    return rooms


@router.put("/web_data")
def get_web_data(data: Data):
    result = {}
    lst_data = [data.room1, data.room2, data.room3]
    for i in range(1,4):
        room = lst_data[i-1]
        check_exception(room)
        collection.update_one({"room_num": i}, {"$set": {"state": room.state}})
        old = collection.find_one({"room_num": i}, {"_id": False})
        if room.is_auto == 1 and (old["state"] != room.state):
            raise HTTPException(status_code=400, detail="Light is in auto mode. Can not manually switched the light.")
        else:
            collection.update_one({"room_num": i}, {"$set": {"state": room.state}})
        collection.update_one({"room_num": 1}, {"$set": {"brightness": percentage_to_binary(room.brightness)}})
        result[f"room{i}"] = collection.find_one({"room_num": i}, {"_id": False})
    return {"results": result}




@router.post("/update_web_data")
def update_web_data():
    pass