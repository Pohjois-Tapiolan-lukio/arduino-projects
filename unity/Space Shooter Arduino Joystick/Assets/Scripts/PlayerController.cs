using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI; // for joystick control
using System.IO.Ports; // joystick
using System;
using System.Text;

[System.Serializable]
public class Boundary
{
	public float xMin, xMax, zMin, zMax;
}

public class PlayerController : MonoBehaviour {


	private Rigidbody rb;


	public float speed;
	public float tilt;
	public Boundary boundary;

	public GameObject shot;
	public Transform shotSpawn;
	public float fireRate;

	private float nextFire;

	string[] delimitter = { ","};

	//public SerialPort serial;
	//SerialPort serial;
	SerialPort serial = new SerialPort ("/dev/cu.usbmodem1411",115200);  // add for the joystick


	// Use this for initialization
	void Start () {



		serial.Open ();  // add for joystick

		rb = GetComponent<Rigidbody> ();

		if (serial.IsOpen) {
			Debug.Log ("USBSerial is now open!");
		} else {
			Debug.LogError ("USBSerial is not open!");
		}


	}
	void Update (){
		if (Input.GetButton ("Jump") && Time.time > nextFire) {
			nextFire = Time.time + fireRate;
			Instantiate (shot, shotSpawn.position, shotSpawn.rotation);
		}
	}

	public string ReadFromArduino(){
		try{
			string joystickValues = serial.ReadLine ();

			serial.BaseStream.Flush ();
			serial.DiscardInBuffer();
			return joystickValues;
		}

		catch{
			return "512,512,1";
		}
	
	} 
	void FixedUpdate () {
		//float moveHorizontal = Input.GetAxis ("Horizontal");
		//float moveVertical = Input.GetAxis ("Vertical");

		// read serial for the joystick starst here


		//string joystickValues = serial.ReadLine (); // ReadFromArduino();
		string joystickValues = ReadFromArduino();
		string[] joystickVector = joystickValues.Split (delimitter, StringSplitOptions.RemoveEmptyEntries);
		//Debug.Log (joystickValues);
		Debug.Log ("0: "+joystickVector[0]+" 1: "+joystickVector[1]);

		if (joystickVector.Length > 2) {
			
			float[] floatVals = new float[3];

			for (int i = 0; i < 3; i++) {
				//foreach (var value in joystickVector) {
				int intVal;
				bool success = Int32.TryParse (joystickVector [i], out intVal);
				if (success) {
					if (i < 2) {
						floatVals [i] = (float)intVal;
						floatVals [i] = floatVals [i] / 512f - 1;
					
					} else {
						floatVals [i] = 1f - (float)intVal;
					}
				} else {
					if (i != 2) {
						floatVals [i] = 0f;
					} else {
						floatVals [i] = 0f;
					}
				}
			}
			Debug.Log ("0f: "+floatVals[0]+" 1f: "+floatVals[1]+" 2f: "+floatVals[2]);

			Vector3 movement = new Vector3 (-floatVals [1], 0.0f, floatVals [0]);
			rb.velocity = movement*speed;

			// Matti added:
			if (floatVals[2] == 1f && Time.time > nextFire) {
				nextFire = Time.time + fireRate;
				Instantiate (shot, shotSpawn.position, shotSpawn.rotation);
			}

		} else {
			Vector3 movement = new Vector3 (0.0f, 0.0f, 0.0f);
			rb.velocity = movement*speed;
		}
	

		//serial.BaseStream.Flush ();



		//Vector3 movement = new Vector3 (moveHorizontal, 0.0f, moveVertical);



		rb.position = new Vector3 
		(
				Mathf.Clamp(rb.position.x, boundary.xMin, boundary.xMax),
				0.0f,
				Mathf.Clamp(rb.position.z, boundary.zMin, boundary.zMax)

		);

		rb.rotation = Quaternion.Euler (0.0f, 0.0f, rb.velocity.x*-tilt);
	}
}
