#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define NUM_VIDEOS 149

int main(){
	
	char ** videos_arr= malloc(NUM_VIDEOS * sizeof(char *));
	char * line_buf= NULL;
	size_t size= 0;
	FILE * video_names= fopen("video_names", "r");
	int j=0;

	//stores the names of each video into videos_array
	for( ; j < NUM_VIDEOS+1; j++){
		getline(&line_buf, &size, video_names);
		int len= strlen(line_buf);
		line_buf[len-1]='\0';
		videos_arr[j]= malloc(100 * sizeof(char));	//there should not be a video name over 100 characters
		strcpy(videos_arr[j], line_buf);
	}

	char * command_buf= malloc(sizeof(char) * 100);

	char ** vlc_command= malloc(sizeof(char *) * 2);
	vlc_command[0]= malloc(sizeof(char) * (strlen("vlc")+1));
	strcpy(vlc_command[0], "vlc");
	vlc_command[1]= malloc(sizeof(char) * (strlen("output.mp4")+1));
	
	//output.mp4 will be the video file after being fuzzed by radamsa
	strcpy(vlc_command[1], "output.mp4");
	
	while(1){
		//gets a random video from a usb drive
		int index= rand()%NUM_VIDEOS-1;
		//path of where the videos are located - change this!
		strcpy(command_buf, "cat /media/jordan/KINGSTON/files/mp4/");
		strcat(command_buf,videos_arr[index]);
		strcat(command_buf," | radamsa > output.mp4");
		
		pid_t child;
		int status;
		
		if((child= fork()) == 0){
			system(command_buf);	//contains the command to fuzz video
			execvp(vlc_command[0], vlc_command);	//contains the command to play the fuzzed video in vlc
			exit(0);
		}
		else {
			sleep(5);
			kill(child, SIGKILL);
			waitpid(child, NULL, WUNTRACED);
		}

		char * line= NULL;
		size_t size= 0;

		//this is redirect the output from ls into a file called dir_list
		system("ls > dir_list");
		FILE * dir_list= fopen("dir_list", "r");
		int j= 0;
		
		//j represents the number of files in the directory
		while((status= getline(&line, &size, dir_list)) != -1){
			j++;
		}
		//if j>5, that means a core dump was created, in other words, vlc crashed
		if(j > 5){
			free(vlc_command[0]);
			free(vlc_command[1]);
			free(vlc_command);
			free(line);
			fclose(dir_list);
			free(line_buf);
			for(i=0; i < 150; i++){
				free(videos_arr[i]);
			}
			free(videos_arr);
			return 0;
		}

	}
}
