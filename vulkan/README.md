## 2 process Vulkan Triangle


- Install dependencies ```./dependencies.sh```
- Make ```./make.sh```

To run:
- Have the kernel module loaded
- Have 2 terminals open ***Monitor*** & ***Tri***
- in terminal ***Monitor*** ```./monitor-run.sh```
- in terminal ***Tri*** ```./tri-run.sh```
- Press ```<enter>``` in terminal ***Tri***
- Press ```<enter>``` in terminal ***Monitor***
    - **You should see a prompt to enter a loop**, *wait here in this terminal*.
- Press ```<enter>``` in terminal ***Tri***
    - This step should take quite a while, you'll know ***it's ready when you see a prompt to enter a loop***
- When ***Tri*** is ready to enter a loop, press ```<enter``` on ***Monitor***
- You may now repeatedly press ```<enter>``` on ***Tri*** to run the next iteration of the render loop
