from fastapi import APIRouter, Body, HTTPException
from fastapi.encoders import jsonable_encoder
from pydantic import BaseModel
from database.mongo_connection import *

router = APIRouter(
    prefix="/",
    tags=["rooms"],
    responses={404: {"description": "Not found"}},
)

class Room(BaseModel):
    """ Room model """
    room_num: str
    state: int
    brightness: int
    is_auto: int


def binary_to_percentage(binary):
    """ Convert binary to percentage """
    return round(binary / 2.55)


@router.get("/rooms")
def get_all_rooms():
    """ Get all rooms """
    rooms = collection.find({"room_num":  True})
    return rooms

@router.get("/update_rooms")
def update_all_rooms():
    """ Update all rooms """
    rooms = collection.find({"room_num":  True})
    return rooms


@router.get("/web_data")
def get_web_data():
    pass


@router.post("update_web_data")
def update_web_data():
    pass