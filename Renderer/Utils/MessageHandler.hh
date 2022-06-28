#ifndef _MESSAGE_HANDLER_HH_
#define _MESSAGE_HANDLER_HH_

using namespace std;

/**     Marcel generic Message handling  class.
  @author     Pierre Lena
  @version    0.0.1*/

const int MASK_1                   = 0xFF000000;
const int MASK_2                   = 0x00FF0000;
const int MASK_3                   = 0x0000FF00;
const int MASK_4                   = 0x000000FF;

#include "Core/Color.hh"
#include <cstdarg>

namespace Marcel{

class MessageHandler
{
private:
  /// Buffer to receive the data from the server
  unsigned char receive_buffer[16384];
  /// Buffer to send the data to the server
  unsigned char send_buffer[16384];

  /** Get integer value from receive_buffer
      @param position the position to start at.
   */
  int getInteger(int position);
  /// Set integer value in send_buffer
  int setInteger(int position, int value);

  /// Get word value from receive_buffer
  int getWord(int);
  /// Set word value in send_buffer
  int setWord(int, int);

  /// Get byte value from receive_buffer
  int getByte(int);
  /// Set byte value in send_buffer
  int setByte(int, int);

  /// Set string value in send_buffer
  int setString(int, unsigned char*);
  /// Set 16 bytes value in send_buffer
  int setHash16(int, unsigned char *);
  /** Set block value in send_buffer
      @see setString, setByte, setInteger, setBlock
  */
  int setBlock(int, int, unsigned char *);


public:
  MessageHandler();
  ~MessageHandler();

  /** Create the message
      @param message_type is the kind of message sent to the server
      @param format needs to be something like:
      <LL>
      <LI>%<B>s</B> for string.
      <LI>%<B>b</B> for byte.
      <LI>%<B>i</B> for integer.
      <LI>%<B>w</B> for word.
      <LI>%<B>B</B> for block.
      </LL>
      @param ... The list of the values to set in the message, accordingly to the format
      @return The size of the generated message
  */
  int CreateMessage(const char*format, int message_type, ...);
  int CreateMessage(char **message, const char*format, int message_type, ...);
  int CreateMessage(char **message, const char*format, int message_type, va_list ap);

  /** Read the message
      @param message is the message to store to.
      @param message_type is the kind of message
      @param format needs to be something like:
      <LL>
      <LI>%<B>s</B> for string.
      <LI>%<B>b</B> for byte.
      <LI>%<B>i</B> for integer.
      <LI>%<B>w</B> for word.
      <LI>%<B>B</B> for block.
      </LL>
      @param ... The list of the values to set in the message, accordingly to the format
      @return The kind of the generated message
  */
  int ReadMessage(char *message, int size, const char*format, ...);
  int ReadMessage(char *message, int size, const char*format, va_list ap);

  int   setColor(int position, Color c);
  Color getColor(int position);
  void  setBinaryBlock(int position, int size, unsigned char*);
  void  getBinaryBlock(int position, int size, unsigned char**);

  void  copySent(char **, int size, int position=0);
  void  copyReceived(char **, int size, int position=0);

  char *getSendBuffer();
  char *getReceiveBuffer();
};
}

#endif
