#include <iostream>
#include <string>
#include <unordered_map>
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::to_string;
using std::unordered_map;

bool ValIsInMap(string val, const unordered_map<string, string>& map)
{
    for (const auto& pair : map) {
        if (pair.second == val) {
            return true;
        }
    }

    return false;
}

string Transl(string str)
{
    const unordered_map<string, string> DICT = {
        {"", "[空]"},
        {"text", "文字信息"},
        {"action", "点击事件"},
        {"value", "点击事件内容"},
        {"color", "文字的颜色"},
        {"has_glowing_text", "文字是否发光"},
        {"is_waxed", "是否涂蜡"},
        {"0b", "否"},
        {"1b", "是"},
        {"open_url", "打开网页"},
        {"run_command", "运行命令"},
        {"suggest_command", "在聊天框中输入命令"},
        {"copy_to_clipboard", "复制到剪切板"}
    };

    if (DICT.find(str) != DICT.end()) {
        return DICT.at(str);
    }
    else {
        return str;
    }

    return str;
}

string EscCharConvert(string data)
{
    string _data = "";
    for (char c : data) {
        if (c == '"') {
            _data += R"#(\\")#";
        }
        else {
            _data += c;
        }
    }

    return _data;
}

class Block_entity_data
{
private:

    string front_text = "";
    string color = "";
    string has_glowing_text = "";
    string messages = "";
    string is_waxed = "";
    string clickEvent[4] = { "" };
    string text[4] = { "" };
    string action[4] = { "" };
    string value[4] = { "" };

public:

    string block_entity_data = "";
    const unordered_map<string, string> VALDEF = {
        {"text", ""},
        {"action", ""},
        {"value", ""},
        {"color", "black"},
        {"has_glowing_text", "0b"},
        {"is_waxed", "1b"}
    };

    void Text(int line, string data) {

        text[line] = R"#("text":")#" + data + '"';
    }

    void Action(int line, string data) {

        action[line] = R"#("action":")#" + data + '"';
    }

    void Value(int line, string data) {

        value[line] = R"#("value":")#" + EscCharConvert(data) + '"';
    }

    void Color(string data) {

        color = "color:\"" + data + '"';
    }

    void Has_glowing_text(string data) {

        has_glowing_text = "has_glowing_text:" + data;
    }

    void Is_waxed(string data) {

        is_waxed = "is_waxed:" + data;
    }

    void splice(void) {

        for (int i = 0;i < 4;i++) {
            clickEvent[i] = R"#("clickEvent":{)#" + action[i] + ',' + value[i] + '}';
        }

        messages = "messages:['{" + clickEvent[0] + ',' + text[0] + "}'," +
            "'{" + clickEvent[1] + ',' + text[1] + "}'," +
            "'{" + clickEvent[2] + ',' + text[2] + "}'," +
            "'{" + clickEvent[3] + ',' + text[3] + "}']";

        front_text = "front_text:{" + color + ',' + has_glowing_text + ',' + messages + '}';
        block_entity_data = '{' + front_text + ',' + is_waxed + '}';
    }

};

class Def_Command
{
private:

    string signType = "minecraft:oak_sign";
    string pos = "~ ~1 ~";
    string command = "";

public:

    string Spawn(string block_entity_data) {

        command = "/setblock " + pos + ' ' + signType + block_entity_data;
        return command;
    }

} Command;

class Def_DataReader
{
private:

    string data = "";
    string choose = "0";
    bool loop = false;

    string Title_TranslOrig(string orig) {

        string str = "【" + Transl(orig) + " | " + orig + "】";
        return str;
    }

    string Chooses_NumOrigTransl(const unordered_map<string, string>* numOrig) {

        string str = "";
        for (int i = 0; i < numOrig->size(); i++) {
            string key = to_string(i);
            string val = numOrig->at(key);
            str = str + key + ": " + val + '(' + Transl(val) + "); ";
        }
        str.erase(str.size() - 2);
        return str;
    }

    string UseDefVal_OrigTransl(string orig) {

        string str = "[直接回车]:使用默认值\"" + orig + "\"(" + Transl(orig) + ')';
        return str;
    }

    string AlrUseDefVal_OrigTransl(string orig) {

        string str = "已使用默认参数: \"" + orig + "\"(" + Transl(orig) + ')';
        return str;
    }

public:

    void Text(Block_entity_data* obj, int line) {

        const string _VALDEF = obj->VALDEF.at("text");

        cout << Title_TranslOrig("text") << endl;
        cout << UseDefVal_OrigTransl(_VALDEF) << endl;

        getline(cin, data);
        if (data.empty()) {
            cout << AlrUseDefVal_OrigTransl(_VALDEF) << endl;
            obj->Text(line, _VALDEF);
        }
        else {
            obj->Text(line, data);
        }
    }

    void Color(Block_entity_data* obj) {

        const string _VALDEF = obj->VALDEF.at("color");

        cout << Title_TranslOrig("color") << endl;
        cout << UseDefVal_OrigTransl(_VALDEF) << endl;

        getline(cin, data);
        if (data.empty()) {
            cout << AlrUseDefVal_OrigTransl(_VALDEF) << endl;
            obj->Color(_VALDEF);
        }
        else {
            obj->Color(data);
        }
    }

    void Value(Block_entity_data* obj, int line) {

        const string _VALDEF = obj->VALDEF.at("value");

        cout << Title_TranslOrig("value") << endl;
        cout << UseDefVal_OrigTransl(_VALDEF) << endl;

        getline(cin, data);
        if (data.empty()) {
            cout << AlrUseDefVal_OrigTransl(_VALDEF) << endl;
            obj->Value(line, _VALDEF);
        }
        else {
            obj->Value(line, data);
        }
    }

    void Action(Block_entity_data* obj, int line) {

        const string _VALDEF = obj->VALDEF.at("action");
        const unordered_map<string, string> numOrig = {
            {"0", "open_url"},
            {"1", "run_command"},
            {"2", "suggest_command"},
            {"3", "copy_to_clipboard"}
        };

        cout << Title_TranslOrig("action") << endl;
        cout << Chooses_NumOrigTransl(&numOrig) << endl;
        cout << UseDefVal_OrigTransl(_VALDEF) << endl;

        do {
            loop = false;
            getline(cin, choose);
            if (numOrig.find(choose) != numOrig.end()) {
                obj->Action(line, numOrig.at(choose));
            }
            else if (ValIsInMap(choose, numOrig)) {
                obj->Action(line, choose);
            }
            else if (choose.empty()) {
                cout << AlrUseDefVal_OrigTransl(_VALDEF) << endl;
                obj->Action(line, _VALDEF);
            }
            else {
                cout << "参数\"" << choose << "\"无效!" << endl;
                loop = true;
            }
        } while (loop);
    }

    void Has_glowing_text(Block_entity_data* obj) {

        const string _VALDEF = obj->VALDEF.at("has_glowing_text");
        const unordered_map<string, string> numOrig = {
            {"0", "0b"},
            {"1", "1b"}
        };

        cout << Title_TranslOrig("has_glowing_text") << endl;
        cout << Chooses_NumOrigTransl(&numOrig) << endl;
        cout << UseDefVal_OrigTransl(_VALDEF) << endl;

        do {
            loop = false;
            getline(cin, choose);
            if (numOrig.find(choose) != numOrig.end()) {
                obj->Has_glowing_text(numOrig.at(choose));
            }
            else if (ValIsInMap(choose, numOrig)) {
                obj->Has_glowing_text(choose);
            }
            else if (choose.empty()) {
                cout << AlrUseDefVal_OrigTransl(_VALDEF) << endl;
                obj->Has_glowing_text(_VALDEF);
            }
            else {
                cout << "参数\"" << choose << "\"无效!" << endl;
                loop = true;
            }
        } while (loop);
    }

    void Is_waxed(Block_entity_data* obj) {

        string _VALDEF = obj->VALDEF.at("is_waxed");
        const unordered_map<string, string> numOrig = {
            {"0", "0b"},
            {"1", "1b"}
        };

        cout << Title_TranslOrig("is_waxed") << endl;
        cout << Chooses_NumOrigTransl(&numOrig) << endl;
        cout << UseDefVal_OrigTransl(_VALDEF) << endl;

        do {
            loop = false;
            getline(cin, choose);
            if (numOrig.find(choose) != numOrig.end()) {
                obj->Is_waxed(numOrig.at(choose));
            }
            else if (ValIsInMap(choose, numOrig)) {
                obj->Is_waxed(choose);
            }
            else if (choose.empty()) {
                cout << AlrUseDefVal_OrigTransl(_VALDEF) << endl;
                obj->Is_waxed(_VALDEF);
            }
            else {
                cout << "参数\"" << choose << "\"无效!" << endl;
                loop = true;
            }
        } while (loop);
    }

} DataReader;

int main()
{
    Block_entity_data obj;

    for (int i = 0; i < 4; i++) {

        cout << "***第 " << i + 1 << " 行***" << endl;

        DataReader.Text(&obj, i);
        DataReader.Action(&obj, i);
        DataReader.Value(&obj, i);
    }

    DataReader.Color(&obj);
    DataReader.Has_glowing_text(&obj);
    DataReader.Is_waxed(&obj);

    obj.splice();

    cout << Command.Spawn(obj.block_entity_data) << endl;

    system("pause");
    return 0;
}