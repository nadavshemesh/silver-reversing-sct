#include "sctasm.h"

void build_data_from_link_table(sct_f* sct) {
    int link_offset = (sct->structure->link_table_off)+4;
    int links_num = ((sct->structure->link_table_size)/4);
    // printf("links num: %d\n", links_num);
    int data_ref_size = 0;
    int ref_is_data[links_num];
    int ref_count[links_num];
    int refs[links_num];

    fseek(sct->file, link_offset, SEEK_SET);
    fread(&refs, 4, links_num, sct->file);

    for(int i=0; i < links_num; i++) { ref_is_data[i] = 0; ref_count[i] = 0; }
    // check and convert to data offset
    for(int i=0; i < links_num; i++) {
        // printf("type: %03x\n", refs[i] & 0xFF000000);
        if((refs[i] & 0xFF000000) == 0x40000000) {
            refs[i] &= 0x00FFFFFF;
            // printf("addr: %06x\n", refs[i]);
            ref_is_data[i]++;
        }
    } 

    // count refs to data by id
    for(int i=0, j=0; i < links_num; i++) {
        if(ref_is_data[i] > 0) {
            int in_code_offset = refs[i]*4+0x24;
            // printf("addr: %06x\n", in_code_offset);
            int data_id;
            fseek(sct->file, in_code_offset, SEEK_SET);
            fread(&data_id, 4, 1, sct->file);

            // printf("data id: %d\n", data_id);
            ref_count[data_id]++;
        }
    }

    // count distinct refs
    for(int i=0; i < links_num; i++) { if(ref_count[i] > 0) data_ref_size++; } 
    // printf("data_ref_size: %08x\n", data_ref_size);

    // create data objects
    data_obj data_arr[data_ref_size];
    for(int i=0, j=0; i < links_num; i++) {
        if(ref_count[i] > 0) {
            data_arr[j].id = i;
            // printf("data id: %d\n", i);
            data_arr[j].references = ref_count[i];
            // printf("ref count: %d\n", ref_count[i]);
            j++;
        }
    }
    // sort data_arr by id
    qsort(data_arr, data_ref_size, sizeof(data_obj), compare_data_obj_ids);

    // calculate data obj size and data
    for(int i=0; i < data_ref_size; i++) {
        int data_section_offset = (sct->structure->data_sec_off) + 4;
        int data_offset = data_section_offset + data_arr[i].id*4;
        int byte_size = 0;

        if(i == data_ref_size-1) { // last item
            byte_size = ((link_offset)-data_offset);
        } else {
            byte_size = (data_arr[i+1].id - data_arr[i].id)*4;
        }

        data_arr[i].byte_size = byte_size;
        int data[byte_size];

        fseek(sct->file, data_offset, SEEK_SET);
        fread(&data, 1, byte_size, sct->file);
        // printf("%08x\n", data[0]);

        //TODO: ASM DATA
        char prefix[] = "VAR_";
        char name[sizeof(prefix)+4];
        sprintf(name, "%s%d", prefix, data_arr[i].id);
        data_arr[i].name = aapts(name);
        data_arr[i].bin_data = w_malloc(byte_size);
        memset(data_arr[i].bin_data, 0, byte_size);
        memcpy(data_arr[i].bin_data, data, byte_size);
        // memcpy(data_arr[i].asm_data, asm_data, byte_size);
    }

    sct->data_objs_size = data_ref_size;
    sct->data_section = w_malloc(sizeof(data_obj*));
    *sct->data_section = w_malloc(data_ref_size*sizeof(data_obj));
    memcpy(*sct->data_section, data_arr, data_ref_size*sizeof(data_obj));
    // print_data_obj((&data_objs[5]));

    // print_data_obj(&(sct->data_section)[5]);
}