# TODO to finish the EndTech Seed Finder

Currently our goals heavily expanded from a basic single threaded implementation to a more dynamic one.
To simplify working together I will line out areas that specific tasks belong to and structure them here.
So far we have

## Generation Tasks

Generation tasks encompass all tasks that are directly related to doing calculations that will reveal something about the generated seed.

### Finished Generation Tasks

- Make Stronghold generation
- Make Biome generation

### TODO Generation Tasks

- Make Desert Temples potential generation
- Make Witch Hut potential generation
- Make Monuments potential generation
- Make Outpost generation (needs further code reviewing)
- Make Village generation (needs further code reviewing)
- Make Slime Chunks generation
- Make Fortress generation

## Search Tasks

Search Tasks are all tasks related to the structure of the search itself.
Currently the plan is it to search through all 48 bits of structure seeds and then to expand the ones that have potential.

### TODO Search Tasks

- Make Potential Search for seed range
  - Define Potential: is double temple/monument required?  
   Whats the limiting range for a end portal to be considered near a structure?
  - Define which parts are part of the potential search and which arent  
   Is Outpost generation completely dependant upon the full seed due to them generating only near villages?
  - Make the defined search for a specific seed range
  - Make a format to store the results in
- Make Expanded search for found potential seed
  - Define Expanded search: define the exact requirements for the search
  - Define which part of generation belongs to potential and expanded search (overlaps with above)
  - Make the defined search for specified structure seed
  - Make a format to store the results in

## Computation Distribution Tasks

Computation distribution tasks includes all tasks that deal with being able to distribute the calculation over as many computation resources as possible.
This includes parralelizing such as multi-threading, using multiple processes or even using multiple PCs.
Currently the idea is it to have a client/server relationship between task providers and takers.
The takers will communicate with the providers through some sort of socket.
Takers themselves can then provide parts of the tasks to smaller takers in their environment.
This way multiple systems can connect to the central task provider and hand their calculations to multiple threads on their system.
While we can also have a single taker system that just takes the entire seed width.
If we spread this calculation across the internet we need some form of authentification to prevent someone from messing up our calculations.  
I imagine a tree structure of Calculators, Reproviders and the Root Provider.
Both the calculators and providers have the same goal. That is to solve as many tasks from their queue as quickly as possible.
The calculators approach this actually crunching the numbers while the providers spread across takers.
As takers their responsibility is it to never let the queue run completely empty - so that they can keep providing or calculating.

### TODO Computation Distribution Tasks

- Make computation queue that contains possible tasks
- Make computation divider that divides the possible larger tasks into subtasks for takers
- Make computation requester tht checks if the queue is approaching its closure and requests new computation tasks
- Make computation tracker that tracks by which taker calculations were/are made
- Make socket communication for the client/server architecture
  - Heartbeat, Authentification, Computation Task Providing, Results Providing
- Make computation balancer that balances the distribution of potential search tasks and expanding search tasks among the takers
- Make computation redistributor that redistributes a task from a taker that takes too long or lost connection
