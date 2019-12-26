#include "fileencryption.h"

//FileEncryption::FileEncryption(ui64 key)
//  :des(key)
//{    
//}

int FileEncryption::encrypt(string input)
{
    return cipher(input, false);
}

int FileEncryption::decrypt(string input)
{
    return cipher(input, true);
}

int FileEncryption::cipher(string input, bool mode)
{
    ifstream ifile;
    ofstream ofile;
    ui64 buffer;
    
    ifile.open(input.c_str(),  ios::binary | ios::in | ios::ate);
  //  cout<<input.c_str()<<endl;
    if(mode == false)
    ofile.open("./encryptFile", ios::binary | ios::out);
    else
    ofile.open("./resourceFile", ios::binary | ios::out);
    ui64 size = ifile.tellg();
    //cout<<"size"<<size<<endl;
    ifile.seekg(0, ios::beg);

    ui64 block = size / 8;
    if(mode) block--; 
   // cout<<"block:"<<block<<endl;
    for(ui64 i = 0; i < block; i++)
    {
        ifile.read((char*) &buffer, 8);

        if(mode)
            buffer = des.decrypt(buffer);
        else
            buffer = des.encrypt(buffer);

        ofile.write((char*) &buffer, 8);
    }

    if(mode == false)
    {
        // 需要的填充量
        ui8 padding = 8 - (size % 8);

        // 填充不能为零，填充满块
        if (padding == 0)
            padding  = 8;

        // 读取剩余文件
        buffer = (ui64) 0;
        if(padding != 8)
            ifile.read((char*) &buffer, 8 - padding);

        // 填充块为1后跟0
        ui8 shift = padding * 8;
        buffer <<= shift;
        buffer  |= (ui64) 0x0000000000000001 << (shift - 1);

        buffer = des.encrypt(buffer);
        ofile.write((char*) &buffer, 8);
    }
    else
    {
        //读取文件最后一行
        ifile.read((char*) &buffer, 8);
        buffer = des.decrypt(buffer);

        //文件中的填充量
        ui8 padding = 0;

        // 检查并记录结尾的填充
        while(!(buffer & 0x00000000000000ff))
        {
            buffer >>= 8;
            padding++;
        }

        buffer >>= 8;
        padding++;

        if(padding != 8)
            ofile.write((char*) &buffer, 8 - padding);
    }

    ifile.close();
    ofile.close();
    return 0;
}
