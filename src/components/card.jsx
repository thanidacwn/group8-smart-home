import React from "react";
import Switch from './Switch'
import '../css/Card.css'

const Card = ({brightness,is_auto,state}) =>{
    let routeimg = "https://th.bing.com/th/id/R.058d45438686e7f81101eebb8f5fd1bd?rik=oU62Xmn4FlzOsw&riu=http%3a%2f%2fimages.clipartpanda.com%2flight-bulb-drawing-HiRes.jpg&ehk=FUUgpT4ZnN5LEt8yDUC1JNwJfW2TdTI98ORcL83HVbY%3d&risl=&pid=ImgRaw&r=0";
    const check_auto = is_auto => {
        if (is_auto) {
            if (brightness > 0) {
                // on
                routeimg = "https://www.netclipart.com/pp/m/9-94774_lightbulb-clipart-classroom-clip-art-light-bulb-off.png"
            }
        }
        else {
            if(state){
                routeimg = "https://www.netclipart.com/pp/m/9-94774_lightbulb-clipart-classroom-clip-art-light-bulb-off.png"
            }
        }
        return routeimg
    }
    return (
        <div>
            <img src = {check_auto(is_auto)} />
            <div>
                <h2>State</h2>
                <Switch />
            </div>
            <div>
                <h2>Brightness</h2>
                <input type="range" min="0" max="255" id="myRange" onChange={e => {console.log(e.target.value)}}/>
            </div>
            <div>
                <h2>Auto</h2>
                <Switch />
            </div>
        </div>    
        
    )
} 

export default Card