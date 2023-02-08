import React from "react";
import Form from 'react-bootstrap/Form';

function Switch({state}) {
  return (
    <Form>
      <Form.Check 
        type="switch"
        id="custom-switch"
        onChange={e=>{state = (e.target.checked)}}/>
    </Form>
  );
}
export default Switch;
