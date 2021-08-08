int isdigit(int c){
	return (c >= '0' && c <= '9');
}

// extracts the last positive integral value of string s
// returns...
// on success, the int value scanned
// -1, if the string is null, empty, or not terminated by a number
int extractLastIntegral(const char* s) {
    int value = -1;
    if (s && *s) {  // don't parse null and empty strings
        const char *scanstr = s + strlen(s) - 1;
        while (scanstr > s && isdigit(*(scanstr-1))) {
            scanstr--;
        }
        sscanf(scanstr,"%d", &value);
    }
    return value;
}



void saveSettings(){
#ifndef POK_SIM    
    mkdir("/joe2/", 0777); // make sure folder exist
    File f;
    
    if(f.openRW("/joe2/settings.ini",1,0)){
        char tempText[20]={0};
        sprintf(tempText,"volume = %d\n",myVolume);
        f.write(tempText, strlen(tempText));
        sprintf(tempText,"border = %d\n",bg.screenTop);
        f.write(tempText, strlen(tempText));
        f.close();
    }
#endif
}

void loadSettings(){
#ifndef POK_SIM
    printf("trying to load settings\n");


    char txtLine[64];
    char tempChar;
    int index = 0;
    mkdir("/joe2/", 0777); // make sure folder exist
    
    File f;
    
    if(f.openRO("/joe2/settings.ini")){
        printf("settings file opened\n");

        while(f.read(&tempChar, 1)){

    //        printf("%c", tempChar);

            if (tempChar == EOF) {
                txtLine[index] = '\0';
                break;
            }
    
            if (tempChar == '\n' || tempChar == '\r') {
                txtLine[index] = '\0';
                printf("New Line\n");
                index=0;
                if(strncmp(txtLine, "volume", strlen("volume"))==0){
                    myVolume = extractLastIntegral(txtLine);
                    printf("Volume = %d\n", myVolume);
                }
                if(strncmp(txtLine, "border", strlen("border"))==0){
                    bg.screenTop = extractLastIntegral(txtLine);
                    printf("Border = %d\n", bg.screenTop);
                }

                continue;
            }else{
                txtLine[index] = (char)tempChar;
            }
            index++;
        }
        
    }else{
        printf("file failed\n");
        myVolume = 10;
        bg.screenTop = 0;
        saveSettings();
    }
#endif
}
