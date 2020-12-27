import json

file_object = open('Live_0392.json')
file_context = file_object.read()
file_object.close()

json = json.loads(file_context)
beat_time = 60.0/110/4

frame_time = beat_time / 5
str1 = "{"
for i in json:
    frame = (i['timing_sec']+0.055)/frame_time
    start_frame_int = round(frame)
    end_frame_int = start_frame_int
    if i["position"]>=5:
        if i["effect"] == 3:
            end_frame_int = round(frame+i["effect_value"]/frame_time)
        print("{"+str(start_frame_int)+","+str(end_frame_int)+"}, ")
        
str1 = str1[:2] + " },"
# print(str1)
            
    # noteinfo = str(frame_int)
            # noteinfo += ",4"
        # else:
            # noteinfo += ",3"
    # else:
        # if i["effect"] == 3:
            # noteinfo += ",1"
        # else:
            # noteinfo += ",2"
    # print(noteinfo)
    # if i["effect"] == 3:
        # if i["position"]<5:
            # end_info += ",4"
        # else:
            # end_info += ",1"
        # print(end_info)3