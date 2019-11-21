# Flæðirit og sauðakóði


Sauðakóðinn okkar:
```

travel history = list

travel struct {
	time,
	left motor,
	right motor 
}

get driving data {
	travel = travel struct
	travel's left motor = left motor velocity
	travel's right motor = right motor velocity

	if travel's left motor velocity is more than 27% {
		set travel's left motor to 30%
	}

	if travel's right motor velocity is more than 27% {
		set travel's right motor to 30%
	}

	if travel's motors are set to reverse {
		multiply values by -1
	}

	reset timer
	return travel
}

main {
	forever {
		if searching {
			take snapshot

			if largestObject exitst {
				x = largestObject.centerX - 157
				y = largestObject.cenerY - 157

				distance = object distance in cm

				if distance is more than 15 cm or less than 0 {
					set speed to 30%
					if x / 40 == 0 {
						if "going" wasn't "forwards" {
							driving data = get driving data
							put driving data to travel history
						}
						going = forwards

						left and right motors go forwards
					} else if x is less than 40 {
						if "going" wasn't "right" {
							driving data = get driving data
							put driving data to travel history
						}
						going = right
						left motor goes forwards
						right motor stops
						
					} else {
						is "going" wasn't going "left" {
							driving data = get driving data
							put driving data to travel history
						}
						going = left
						left motor stops
						right motor goes forwards
					}
				}
			} else if distance is more than 3,8 cm {
				set speed to 10%
				left and right motors go forwards
			} else {
				left and right motors stop
				searching = false
				continue
			}
		} else {
			if "going" wasn't "nowhere" {
				driving data = get driving data
				put driving data into travel history
			}

			going = nowhere

			left and right motors stop
		}
		is not searching {
			spin claw to reverse direction
			reset timer
			wait 5 seconds
			make robot turn around (180 degrees)

			wait 6 seconds
			break infinite loop
		}

	}

	for (i = length of travel history; run while i is more or equal to 0; i--) {
		driving data = travel history [x]
		// We need to flip the motors if we want the robot to go to the correct direction back
		left motor = right motor from driving data
		right motor = left motor from driving data

		reset timer
		
		wait for (time from driving data) milliseconds

		left and right motors stop
	}

	
}

 ```

![Flæðirit](./img/flowchart.png)
