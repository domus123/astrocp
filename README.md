# astrocp
A brain for multiple agents



# Especifications 
Astro is a framework that can hold multiple autonomous agents and control they position in the same time.
With astro you can control multiple autonomous systems at once in a endpoint and:
  * Create your custom sensoring/physical models and use on the go.
  * Create multiple agent SLAM
  * Pathfind/route optization for multiples agents
  
# Server side 

The astro brains hold the job of receiving sensoring data from de clients and do all the heavy math.

# client side

In the client size you have to collect the sensoring data and send to the server, while receiving back commands.
  
