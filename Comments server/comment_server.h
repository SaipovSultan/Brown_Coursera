#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <utility>
#include <map>
#include <optional>
#include <unordered_set>

struct HttpRequest {
    std::string method, path, body;
    std::map<std::string, std::string> get_params;
};

std::pair<std::string, std::string> SplitBy(const std::string& what, const std::string& by);

template<typename T>
T FromString(const std::string& s);

std::pair<std::size_t, std::string> ParseIdAndContent(const std::string& body);

struct LastCommentInfo {
    std::size_t user_id, consecutive_count;
};

enum class HttpCode {
    Ok = 200,
    NotFound = 404,
    Found = 302,
};

struct HttpHeader {
    std::string name, value;
};

class HttpResponse {
public:
    explicit HttpResponse(HttpCode code) : httpCode(code) {}

    HttpResponse& AddHeader(std::string name, std::string value);
    HttpResponse& SetContent(std::string a_content);
    HttpResponse& SetCode(HttpCode a_code);

    friend std::ostream& operator << (std::ostream& output, const HttpResponse& resp);
private:
    std::vector<HttpHeader> httpHeaders;
    std::string httpContent;
    HttpCode httpCode;
};

class CommentServer {
private:
    std::vector<std::vector<std::string>> comments_;
    std::optional<LastCommentInfo> last_comment;
    std::unordered_set<std::size_t> banned_users;

public:
    void ServeRequest(const HttpRequest& req, std::ostream& os);
    HttpResponse ServeRequest(const HttpRequest& req);
};

std::ostream& operator<<(std::ostream& output, const HttpHeader& h);

bool operator==(const HttpHeader& lhs, const HttpHeader& rhs);

struct ParsedResponse {
    int code;
    std::vector<HttpHeader> headers;
    std::string content;
};

std::istream& operator >>(std::istream& input, ParsedResponse& r);
